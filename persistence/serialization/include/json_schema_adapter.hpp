#pragma once

#include <nlohmann/json.hpp>
#include "state_report.hpp"
#include "persistence_helpers.hpp"
#include <concepts>
#include <type_traits>
#include <functional>
#include "id_generator.hpp"
namespace Persistence::Adapter
{
/* The idea here is to turn reports DTO's into valid json objects
in order to validate them against the schemas provided in "report_schema.hpp"
A consideration to have in mind -
* Fields of an Id-type (see "domain/include/ids.hpp") will be represented as simple numeric
values, this is, we'll just extract it's internal unsigned integer and put it
straight into de json field
* Fields of an OptionalDate type("at domain/include/types.hpp") will be turned as strings,
wether an std::nullopt will be te equivalent of an empty string or a "NONE" string is yet
to discuss with my inner voices
*/
// ---------------------------------Serialization---------------------------------
nlohmann::json serialize_date(const Domain::OptionalDate& date);
Domain::OptionalDate deserialize_date(const nlohmann::json& field);
nlohmann::json to_json(const Domain::State::TaskReport& report);
nlohmann::json to_json(const Domain::State::SectionReport& report);
nlohmann::json to_json(const Domain::State::ManagerFullReport& report);
template <typename Report>
std::vector<nlohmann::json> serialize_reports_array(const std::vector<Report>& reports) {
    std::vector<nlohmann::json> output_field;
    std::ranges::transform(
        reports,
        std::back_inserter(output_field),
        [](const Report& report) {
            return to_json(report);
        }
    );
    return output_field;
}
// -------------------------------Deserialization-------------------------------
template <Domain::ID id>
std::vector<id> deserialize_ids_array(const nlohmann::json& array) {
    std::vector<id> ids;
    ids.reserve(array.size());
    std::ranges::transform(
        array,
        std::back_inserter(ids),
        [](const uint64_t x) {return id{x};}
    );
    return ids;
} 

template <typename Converter, typename Report>
concept JsonDeserializer =
    std::regular_invocable<Converter, const nlohmann::json&> && //calleable with a const json& argument
    std::same_as<std::invoke_result_t<Converter, const nlohmann::json&>, Report>;


template <typename Report, typename Converter>
requires JsonDeserializer<Converter, Report>
std::vector<Report> deserialize_reports_array(
    const nlohmann::json& reports, Converter converter) {
    
    std::vector<Report> output_vector;
    output_vector.reserve(reports.size());
    std::ranges::transform(
        reports,
        std::back_inserter(output_vector),
        converter
    );
    return output_vector;
}
Domain::State::TaskReport task_report_from_json(const nlohmann::json& data);
Domain::State::SectionReport section_report_from_json(const nlohmann::json& data);
template <Domain::ID id>
Domain::State::IDGeneratorReport generator_report_from_json(const nlohmann::json& last_id) {
    Domain::IDGenerator<id> gen;
    gen.set_last_used(last_id.get<uint64_t>());
    return gen.report_state();
}
Domain::State::ManagerFullReport full_report_from_json(const nlohmann::json& data);

}
