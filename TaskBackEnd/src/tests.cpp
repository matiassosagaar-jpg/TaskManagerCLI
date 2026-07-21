#include <iostream>
#include <nlohmann/json.hpp>
#include "json_schema.hpp"
using namespace Schema;
namespace JsonFieldTests {
void test1() {
    // Test 1:  Correct Type - No restrictions
    JsonField field(JsonField::FieldType::Number);
    ValidationResult result = field.validate(42);
    std::cout << "Test N°1" << std::endl;
    std::cout << result.report << std::endl;

    if (!result.success) {
        std::cerr << "Failed validation when should be valid" << std::endl;
        return;
    }
    if (!result.report["errors"].empty()) {
        std::cerr << "Error: 'errors' field overloaded when it should be empty" << std::endl;
        return;
    }
    std::cout << "Test N°1 Succeeded" << std::endl;
}
void test2() {
    // Test 2: Incorrect Type - No restrictions
    JsonField field(JsonField::FieldType::Number);
    ValidationResult result = field.validate("Joaquín");
    std::cout << "Test N°2" << std::endl;
    std::cout << result.report << std::endl;

    if (result.success) {
        std::cerr << "Passed validation when should be invalid" << std::endl;
        return;
    }
    if (result.report["errors"].empty()) {
        std::cerr << "Errors field empty when it should have one" << std::endl;
        return;
    }
    // Checking error received
    auto& err = result.report["errors"][0];

    assert(err["type"] == "type_mismatch");
    assert(err["expected"] == "number");
    assert(err["received"] == "string");
    std::cout << "Test N°2 Succeeded" << std::endl;
}

void test3() {
    // Test 3: One restriction - Doesn't fail
    using namespace Schema;
    JsonField field(JsonField::FieldType::Number);
    field.add_restriction(RangeOfValue<int>{0,100});
    ValidationResult result = field.validate(50);
    std::cout << "Test N°3" << std::endl;
    std::cout << result.report << std::endl;

    if (!result.success) {
        std::cerr << "Failed validation when should be valid" << std::endl;
        return;
    }
    if (!result.report["errors"].empty()) {
        std::cerr << "Error: 'errors' field overloaded when it should be empty" << std::endl;
        return;
    }
    std::cout << "Test N°3 Succeeded" << std::endl;
}

void test4() {
    // Test 4: One restriction - It fails
    JsonField field(JsonField::FieldType::Number);
    field.add_restriction(RangeOfValue<int>{0,100});
    ValidationResult result = field.validate(150);
    std::cout << "Test N°4" << std::endl;
    std::cout << result.report << std::endl;

    if (result.success) {
        std::cerr << "Passed validation when should be invalid" << std::endl;
        return;
    }
    if (result.report["errors"].empty()) {
        std::cerr << "empty error field when there should be one" << std::endl;
        return;
    }
    // Checking error received
    auto& err = result.report["errors"][0];

    assert(err["type"] == "range");
    assert(err["min"] == 0);
    assert(err["max"] == 100);
    std::cout << "Test N°4 Succeeded" << std::endl;
}

void test5() {
    // Test 5: Two restrictions - none fails
    JsonField field(JsonField::FieldType::Number);
    field.add_restriction(RangeOfValue<int>{0,100});
    field.add_restriction(SetOfValues<int>{11,30,20});
    ValidationResult result = field.validate(20);
    std::cout << "Test N°5" << std::endl;
    std::cout << result.report << std::endl;

    if (!result.success) {
        std::cerr << "Failed validation when should be valid" << std::endl;
        return;
    }
    if (!result.report["errors"].empty()) {
        std::cerr << "Errors field overloaded when should be empty" << std::endl;
        return;
    }
    std::cout << "Test N°5 Succeeded" << std::endl;
}

void test6() {
    // Test 6: Two restrictions - one fails
    JsonField field(JsonField::FieldType::Number);
    field.add_restriction(RangeOfValue<int>{0,100});
    field.add_restriction(SetOfValues<int>{11,30,20});
    ValidationResult result = field.validate(40);
    std::cout << "Test N°6" << std::endl;
    std::cout << result.report << std::endl;

    if (result.success) {
        std::cerr << "Passed validation when should be invalid" << std::endl;
        return;
    }
    if (result.report["errors"].empty()) {
        std::cerr << "Errors empty when should at least have one element" << std::endl;
        return;
    }
    // checking error field
    auto& err = result.report["errors"][0];

    assert(err["type"] == "set_of_values");
    std::cout << "Test N°6 Succeeded" << std::endl;
}

void test7() {
    // Test 7: Two restrictions - both fail
    JsonField field(JsonField::FieldType::Number);
    field.add_restriction(RangeOfValue<int>{0,100});
    field.add_restriction(SetOfValues<int>{11,30,20});
    ValidationResult result = field.validate(101);
    std::cout << "Test N°7" << std::endl;
    std::cout << result.report << std::endl;

    if (result.success) {
        std::cerr << "Passed validation when should be invalid" << std::endl;
        return;
    }
    if (result.report["errors"].size() < 2) {
        std::cerr << "less than 2 error messages when should be at least 2" << std::endl;
        return;
    }
    // checking for "correct" errors
    bool range=false;
    bool set=false;

    for (auto& err : result.report["errors"]) {
        if (err["type"]=="range")
            range=true;

        if (err["type"]=="set_of_values")
            set=true;
    }

    assert(range);
    assert(set);
    std::cout << "Test N°7 Succeeded" << std::endl;
}

void test8() {
    // Test 8: Type mismatch cuts validation against restrictions
    JsonField field(JsonField::FieldType::Number);
    field.add_restriction(RangeOfValue<int>{0,100});
    field.add_restriction(SetOfValues<int>{11,30,20});
    ValidationResult result = field.validate("I shouldn't be here");
    std::cout << "Test N°8" << std::endl;
    std::cout << result.report << std::endl;

    if (result.success) {
        std::cerr << "Passed validation when should be invalid" << std::endl;
        return;
    }
    if (result.report["errors"].empty()) {
        std::cerr << "there should be at least un error in the errors array" << std::endl;
        return;
    }
    assert(result.report["errors"][0]["type"] == "type_mismatch");
    assert(result.report["errors"].size() == 1);
    std::cout << "Test N°8 Succeeded" << std::endl;
}
}
void run_jsonField_tests() {
    JsonFieldTests::test1();
    JsonFieldTests::test2();
    JsonFieldTests::test3();
    JsonFieldTests::test4();
    JsonFieldTests::test5();
    JsonFieldTests::test6();
    JsonFieldTests::test7();
    JsonFieldTests::test8();
}

namespace JsonSchemaTests {
void all_valid_fields() {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate({
        {"name", "Matias"},
        {"age", 23} 
    });

    assert(result.success); // terminate if success is false
    auto report = result.report["validation_result"];
    assert(report.size() == 2); // should have 2 reports in my valildation_result, one for each field
    for (const auto& item : report) {
        assert(item["errors"].empty()); //terminate if not empty
    }
    // Verifying correct field names
    bool has_name = false;
    bool has_age = false;

    for (const auto& item : report) {
        if (item["field_name"] == "name")
            has_name = true;

        if (item["field_name"] == "age")
            has_age = true;

        assert(item["errors"].empty());
    }

    assert(has_name);
    assert(has_age);
    std::cout << "Test 1: All valid fields - Passed" << std::endl;
    std::cout << result.report << std::endl;
}

void one_invalid_field() {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate({
        {"name", "Matias"},
        {"age", "23"} 
    });

    assert(!result.success); // terminate if success is true
    auto report = result.report["validation_result"];
    assert(report.size() == 2); // should have 2 reports in my valildation_result, one for each field
    // Verifying correct field names and properties
    bool has_name = false;
    bool has_age = false;

    for (const auto& item : report) {

        if (item["field_name"] == "name") {
            has_name = true;
            assert(item["errors"].empty());
        }

        if (item["field_name"] == "age") {
            has_age = true;

            auto& err = item["errors"]; // error field of "age"
            assert(err.size() == 1); // terminates if it hasn't exactly one error
            assert(err[0]["type"] == "type_mismatch"); // terminates if it's not a type_mismatch error
        }
    }

    assert(has_name);
    assert(has_age);
    std::cout << "Test 2: One Invalid Field - Passed" << std::endl;
    std::cout << result.report << std::endl;
}

void all_invalid_fields() {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate({
        {"name", 23},
        {"age", "Matias"} 
    });

    assert(!result.success); // terminate if success is true
    auto report = result.report["validation_result"];
    assert(report.size() == 2); // should have 2 reports in my valildation_result, one for each field
    // Verifying correct field names and properties
    bool has_name = false;
    bool has_age = false;

    for (const auto& item : report) {

        if (item["field_name"] == "name")
            has_name = true;

        if (item["field_name"] == "age")
            has_age = true;

        auto& err = item["errors"]; // error field
        assert(err.size() == 1); // has only one element
        assert(err[0]["type"] == "type_mismatch"); // is of type "type_mismatch"
    }

    assert(has_name);
    assert(has_age);

    std::cout << "Test 3: All Invalid Fields - Passed" << std::endl;
    std::cout << result.report << std::endl;
}

void missing_field()  {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate({
        {"name", "Matias"}
    });

    assert(!result.success); // terminate if success is true
    auto report = result.report["validation_result"];
    assert(report.size() == 2); // should have 2 reports in my valildation_result, one for each field
    // Verifying correct field names and properties
    bool has_name = false;
    bool has_age = false;

    for (const auto& item : report) {

        if (item["field_name"] == "name") {
            has_name = true;
            assert(item["errors"].empty());
        }

        if (item["field_name"] == "age") {
            has_age = true;

            auto& err = item["errors"]; // error field of "age"
            assert(err.size() == 1); // terminates if it hasn't exactly one error
            assert(err[0]["type"] == "field_not_found"); // terminates if it's not a type_mismatch error
            assert(item["value"].is_null());
        }
    }

    assert(has_name);
    assert(has_age);
    std::cout << "Test 4: One missing field - Passed" << std::endl;
    std::cout << result.report << std::endl;
}

void two_missing_fields() {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate(nlohmann::json::object()); // passing an empty json
    assert(!result.success); // terminate if success is true
    auto report = result.report["validation_result"];
    assert(report.size() == 2); // should have 2 reports in my valildation_result, one for each field
    // Verifying correct field names and properties
    bool has_name = false;
    bool has_age = false;

    for (const auto& item : report) {

        if (item["field_name"] == "name")
            has_name = true;

        if (item["field_name"] == "age")
            has_age = true;

        auto& err = item["errors"]; // error field
        assert(err.size() == 1); // has only one element
        assert(err[0]["type"] == "field_not_found"); // is of type "type_mismatch"
        assert(item["value"].is_null());
    }

    assert(has_name);
    assert(has_age);

    std::cout << "Test 5: All Fields missing - Passed" << std::endl;
    std::cout << result.report << std::endl;
}

void unexpected_field() {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate({
        {"name", "Matias"},
        {"age", 23},
        {"wage", 1100}
    });

    assert(!result.success); // terminate if success is true
    auto report = result.report["validation_result"];
    assert(report.size() == 3); // should have 3 reports in my valildation_result, one for each field and another for the unexpected field;
    // Verifying correct field names and properties
    bool has_name = false;
    bool has_age = false;
    bool has_wage = false;
    for (const auto& item : report) {

        if (item["field_name"] == "name") {
            has_name = true;
            assert(item["errors"].empty());
        }

        if (item["field_name"] == "age") {
            has_age = true;
            assert(item["errors"].empty());
        }

        if (item["field_name"] == "wage") {
            has_wage = true;

            auto& err = item["errors"]; // error field of "age"
            assert(err.size() == 1); // terminates if it hasn't exactly one error
            assert(err[0]["type"] == "unexpected_field"); // terminates if it's not a type_mismatch error
            assert(item["value"].is_null());
        }
    }

    assert(has_name);
    assert(has_age);
    std::cout << "Test 6: Unexpected field - Passed" << std::endl;
    std::cout << result.report << std::endl;
}

void two_unexpected_fields() {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate({
        {"name", "Matias"},
        {"age", 23},
        {"wage", 1100},
        {"status", "single"}
    });

    assert(!result.success); // terminate if success is true
    auto report = result.report["validation_result"];
    assert(report.size() == 4); // should have 3 reports in my valildation_result, one for each field and another for the unexpected field;
    // Verifying correct field names and properties
    bool has_name = false;
    bool has_age = false;
    bool has_wage = false;
    bool has_status = false;
    for (const auto& item : report) {

        if (item["field_name"] == "name") {
            has_name = true;
            assert(item["errors"].empty());
        }

        if (item["field_name"] == "age") {
            has_age = true;
            assert(item["errors"].empty());
        }

        if (item["field_name"] == "wage") {
            has_wage = true;

            auto& err = item["errors"]; // error field of "age"
            assert(err.size() == 1); // terminates if it hasn't exactly one error
            assert(err[0]["type"] == "unexpected_field"); // terminates if it's not a type_mismatch error
            assert(item["value"].is_null());
        }

        if (item["field_name"] == "status") {
            has_status = true;

            auto& err = item["errors"]; // error field of "age"
            assert(err.size() == 1); // terminates if it hasn't exactly one error
            assert(err[0]["type"] == "unexpected_field"); // terminates if it's not a type_mismatch error
            assert(item["value"].is_null());
        }
    }

    assert(has_name);
    assert(has_age);
    assert(has_status);
    assert(has_wage);
    std::cout << "Test 7: Two Unexpected fields - Passed" << std::endl;
    std::cout << result.report << std::endl;
}

void unexpected_and_missing() {
    JsonSchema schema;
    schema.set_field("name", JsonField::FieldType::String); // Implicit conversion from FieldType to JsonField
    schema.set_field("age", JsonField::FieldType::Number);
    
    auto result = schema.validate({
        {"status", "single"},
        {"wage", 1100}
    });

    assert(!result.success); // terminate if success is true
    auto report = result.report["validation_result"];
    assert(report.size() == 4); // should have 3 reports in my valildation_result, one for each field and another for the unexpected field;
    // Verifying correct field names and properties
    bool has_name = false;
    bool has_age = false;
    bool has_wage = false;
    bool has_status = false;
    for (const auto& item : report) {

        if (item["field_name"] == "name") {
            has_name = true;
            auto& err = item["errors"];
            assert(err.size() == 1); 
            assert(err[0]["type"] == "field_not_found"); 
            assert(item["value"].is_null());
        }

        if (item["field_name"] == "age") {
            has_age = true;
            auto& err = item["errors"];
            assert(err.size() == 1); 
            assert(err[0]["type"] == "field_not_found"); 
            assert(item["value"].is_null());
        }

        if (item["field_name"] == "wage") {
            has_wage = true;

            auto& err = item["errors"]; 
            assert(err.size() == 1); 
            assert(err[0]["type"] == "unexpected_field"); 
            assert(item["value"].is_null());
        }

        if (item["field_name"] == "status") {
            has_status = true;

            auto& err = item["errors"]; // error field of "age"
            assert(err.size() == 1); // terminates if it hasn't exactly one error
            assert(err[0]["type"] == "unexpected_field"); // terminates if it's not a type_mismatch error
            assert(item["value"].is_null());
        }
    }

    assert(has_name);
    assert(has_age);
    assert(has_status);
    assert(has_wage);
    std::cout << "Test 8: Two unexpected, and two missing fields - Passed" << std::endl;
    std::cout << result.report << std::endl;
}
} // namespace JsonSchemaTests
void run_JsonSchema_tests() {
    JsonSchemaTests::all_valid_fields();
    JsonSchemaTests::one_invalid_field();
    JsonSchemaTests::all_invalid_fields();
    JsonSchemaTests::missing_field();
    JsonSchemaTests::two_missing_fields();
    JsonSchemaTests::unexpected_field();
    JsonSchemaTests::unexpected_and_missing();
}

int main() {
    run_jsonField_tests();
    run_JsonSchema_tests();

}
