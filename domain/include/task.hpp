#pragma once
#include "types.hpp"
#include "ids.hpp"
#include "state_report.hpp"
#include <chrono>
#include <string>
#include <optional>
#include <format>
namespace Domain 
{
class Task {
public:
    //------------Interface------------

    // CONSTRUCTORS
    Task(
        TaskID id,
        std::string title,
        std::string description,
        Priority priority,
        OptionalDate deadline = std::nullopt,
        SectionID section = SectionID::none()
    );
    ~Task() = default; // data members already know how free their owned resources
    void set_section(SectionID id) {section = id;}
    State::TaskReport report_state() const;
    // BASIC QUERIES
    SectionID get_sectionID() const {return section;}
    bool has_section() const {return section!=SectionID{0};}
    const std::string& get_title() const {return title;}
private:
    TaskID id;
    std::string title;
    std::string description;
    Priority priority;
    SectionID section;
    OptionalDate deadline;
};
}
