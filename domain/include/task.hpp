#pragma once
#include "ids.hpp"
#include <chrono>
#include <string>
#include <optional>
namespace Domain 
{
using Date = std::chrono::year_month_day;
using OptionalDate = std::optional<Date>;
class Task {
public:
    //------------Interface------------
    enum class Priority {
        Low,
        Medium,
        High,
        Urgent
    };
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
