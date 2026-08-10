#include "json_schema_adapter.hpp"

namespace Persistence::Adapter {
using namespace Domain::State;
using json = nlohmann::json;

json serialize_date(const Domain::OptionalDate& date) {
    if (!date)
        return nullptr;
    return std::format("{:%F}", *date);
}

Domain::OptionalDate deserialize_date(const json& field) {
    if (field.is_null())
        return std::nullopt;
    std::stringstream ss(field.get<std::string>());
    Domain::Date date;
    std::chrono::from_stream(ss, "%F", date);
    if (!ss.fail() && date.ok())
        return date;
    throw std::invalid_argument("Failed to serialize date field");
}

json to_json(const TaskReport& report) {
    return {
        {"id", report.id.value},
        {"title", report.title},
        {"description", report.description},
        {"priority", to_str(report.priority)},
        {"deadline", serialize_date(report.deadline)},
        {"section", report.section.value}
    };
}

TaskReport task_report_from_json(const json& data) {
    return {
        Domain::TaskID {data.at("id").get<uint64_t>()},
        data.at("title").get<std::string>(),
        data.at("description").get<std::string>(),
        to_prio(data.at("priority").get<std::string>()),
        deserialize_date(data.at("deadline")),
        Domain::SectionID {data.at("section").get<uint64_t>()}
    };

}

json to_json(const SectionReport& report) {
    return {
        {"name", report.name},
        {"id", report.id.value},
        {"tasks", get_values(report.tasks)}
    };
}

SectionReport section_report_from_json(const json& data) {
    std::vector<Domain::TaskID> task_ids;
    auto input = data.at("tasks").get<std::vector<uint64_t>>();
    task_ids.reserve(input.size());
    std::ranges::transform(
        input,
        std::back_inserter(task_ids),
        [](const uint64_t x) {return Domain::TaskID {x};}
    );
    return {
        data.at("name").get<std::string>(),
        Domain::SectionID {data.at("id").get<uint64_t>()},
        task_ids
    };
}

json to_json(const ManagerFullReport& report) {
    return {
        {"task_reports", serialize_reports_array(report.task_reports)},
        {"section_reports", serialize_reports_array(report.section_reports)},
        {"last_task_id", report.last_task_id.last_used},
        {"last_section_id", report.last_section_id.last_used}
    };
}

ManagerFullReport full_report_from_json(const json& data) {
    return {
        deserialize_reports_array<TaskReport>(
            data.at("task_reports"), task_report_from_json
        ),
        deserialize_reports_array<SectionReport>(
            data.at("section_reports"), section_report_from_json
        ),
        generator_report_from_json<Domain::TaskID>(data.at("last_task_id")),
        generator_report_from_json<Domain::SectionID>(data.at("last_section_id"))
    };
}
} // namespace Persistence::Adapter