#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <algorithm>
#include <memory>
namespace Schema {
// --------------Restrictions--------------
class Restriction {
public:
    virtual ~Restriction() = default;
    virtual bool matches(const nlohmann::json& value) const = 0;
    virtual std::unique_ptr<Restriction> clone() const = 0;
    virtual nlohmann::json description() const = 0;
private:
};
// ---------SetOfValues---------
template <typename T>
class SetOfValues : public Restriction {
public:
    SetOfValues(std::initializer_list<T> values) : values{values} {} // Allows init with init_list
    SetOfValues(const std::vector<T>& values) : values {values} {} // Allows init with a vector
    bool matches(const nlohmann::json& field_value) const override { // I realized the type doesn't matter here, since whatever it takes, it'll check: a==b
        return std::find(values.begin(), values.end(), field_value.get<T>()) != values.end();}
    std::unique_ptr<Restriction> clone() const override {
        return std::make_unique<SetOfValues<T>>(*this);
    }
    nlohmann::json description() const override {
        nlohmann::json description;
        description["type"] = "set_of_values";
        description["allowed_values"] = values;
        return description;
    }
private:
    const std::vector<T> values;
};
// ---------RangeOfValue---------
template <typename T>
class RangeOfValue : public Restriction {
public:
    RangeOfValue(T min, T max) : min{min}, max{max} {
        if (min > max) {
            throw std::invalid_argument("Invalid Range");
        }
    }
    bool matches(const nlohmann::json& field_value) const override {
        T value = field_value.get<T>();
        return value <= max && value >= min;    
    }
    std::unique_ptr<Restriction> clone() const override {
        return std::make_unique<RangeOfValue<T>>(*this);
    }
    nlohmann::json description() const override {
        nlohmann::json description;
        description["type"] = "range";
        description["min"] = min;
        description["max"] = max;
        return description;
    }
private:
    T min;
    T max;
};
// ---------------ValidationReports---------------
struct ValidationResult {
    bool success {true};
    nlohmann::json report {};
    explicit operator bool() const{ // Defining conversion from ValidationResult --> boolean
        return success;
    }
    // --------------------Incomplete Method(add)------------------
/*     
    void add(const ValidationResult& val) {
        if (val.report.is_array())
            throw std::invalid_argument("Cannot add a collection of errors");
        if (!report.is_array())
            throw std::logic_error("Cannot add to Non-Array ValidationResult");
        success = success && val.success;
        report.push_back(val.report); // will fail probably if report is not an array
    } 
*/
};
enum class ValidationErrorType {
    FieldNotFound,
    UnexpectedField
};
std::string error_string(ValidationErrorType err) {
    switch (err) {
    case ValidationErrorType::FieldNotFound:
        return "field_not_found";
    case ValidationErrorType::UnexpectedField:
        return "unexpected_field";
    }   
    throw std::invalid_argument("Invalid enum");
}
ValidationResult make_error(ValidationErrorType err) {
    ValidationResult error;
    error.success = false;
    error.report["value"] = nullptr;
    error.report["errors"] = nlohmann::json::array();
    error.report["errors"].push_back({{"type", error_string(err)}});
    return error;
}
//---------------JsonField--------------
class JsonField {
public:
    enum class FieldType {
        None,
        String,
        Number,
        Boolean,
        Array,
        Object
    };

    static std::string field_to_str(FieldType type) {
        switch (type)
        {
        case FieldType::String : 
            return "string";
        case FieldType::Number:
            return "number";
        case FieldType::Boolean:
            return "boolean";
        case FieldType::Array:
            return "array";
        case FieldType::Object:
            return "object";
        default:
            return "None";
        }
    }
    JsonField() = default;
    JsonField(FieldType json_type) : field_type{json_type} {}
    friend void swap(JsonField&, JsonField&);
    FieldType get_field_type() const {return field_type;}
    // ---------------Copy Semantics---------------

    JsonField(const JsonField& json_field) {
        this->field_type = json_field.field_type;
        for (const auto& pointer : json_field.restrictions) {
            this->restrictions.push_back((*pointer).clone());
        }
    }
    JsonField& operator=(const JsonField& json_field) {
        // use copy constructor with a temporal variable
        JsonField temp(json_field);
        // swap values with it
        swap(*this, temp);
        return *this; // destructors will free the copy that had the previous state of this object
    }

    // -------------Move Semantics-------------

    JsonField(JsonField&& json_field) { // since json_field is an r-value, no one has acces to it, so it's safe to steal it's data 
        this->restrictions = std::move(json_field.restrictions);
        this->field_type = json_field.field_type;
    }

    JsonField& operator=(JsonField&& json_field) {
        swap(*this, json_field);
        return *this;
    }
    ~JsonField() = default;
    void add_restriction(const Restriction& res) {
        restrictions.push_back(res.clone());
    }
    // ----------------Validation-------------
    static JsonField::FieldType get_field_type(const nlohmann::json& value) {
        if (value.is_string()) {
            return JsonField::FieldType::String;
        }

        if (value.is_number()) {
            return JsonField::FieldType::Number;
        }

        if (value.is_boolean()) {
            return JsonField::FieldType::Boolean;
        }

        if (value.is_array()) {
            return JsonField::FieldType::Array;
        }

        if (value.is_object()) {
            return JsonField::FieldType::Object;
        }

        return JsonField::FieldType::None;
    }
    ValidationResult validate(const nlohmann::json& js) const {
        ValidationResult result;
        result.report["value"] = js;
        result.report["errors"] = nlohmann::json::array();
        auto received_type = get_field_type(js);
        if (field_type != received_type) { // If value type error occurs, it won't check restrictions
            result.success = false;
            nlohmann::json error;
            error["type"] = "type_mismatch";
            error["expected"] = field_to_str(field_type);
            error["received"] = field_to_str(received_type);
            result.report["errors"].push_back(error);
            return result; 
        }
        for (const auto& res : restrictions) {
            if (!res->matches(js)) {
                result.success = false;
                result.report["errors"].push_back(res->description());
            }
        }
        return result;
    }
private:
    FieldType field_type {FieldType::None};
    std::vector<std::unique_ptr<Restriction>> restrictions;
};

void swap(JsonField& js1, JsonField& js2) {
    std::swap(js2.restrictions, js1.restrictions);
    std::swap(js2.field_type, js1.field_type);
}

class JsonSchema {
public:
    JsonSchema(std::initializer_list<std::pair<const std::string, JsonField>> fields) :
        fields{fields} {}
    JsonSchema() = default;
    ~JsonSchema() = default;
    void set_field(const std::string& name, JsonField field) {
        fields[name] = std::move(field); // field is passed by value, so we'll use move semantics to not create an additional copy
    }
    ValidationResult validate(const nlohmann::json& js) {
        ValidationResult result;    
        result.report["validation_result"] = nlohmann::json::array();
        for (const auto& field : fields) {
            if (js.contains(field.first)) { // Checking if the tested json has the field
                auto field_result = field.second.validate(js.at(field.first));
                field_result.report["field_name"] = field.first;
                if (!field_result.success) 
                    result.success = false;
                result.report["validation_result"].push_back(field_result.report);
            } else {
                result.success = false;
                auto report = make_error(ValidationErrorType::FieldNotFound).report;
                report["field_name"] = field.first;
                result.report["validation_result"].push_back(report);
            }
        }

        for (const auto& [key,value] :js.items()) {
            if (fields.find(key) == fields.end()) {
                result.success = false;
                auto report = make_error(ValidationErrorType::UnexpectedField).report;
                report["field_name"] = key;
                result.report["validation_result"].push_back(report);
            }
        }
        return result;
    }
private:
    std::unordered_map<std::string, JsonField> fields;
    // ------------Errors-------------------------
};
} // namespace Schema