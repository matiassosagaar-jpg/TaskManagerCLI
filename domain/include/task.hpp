#pragma once
#include "ids.hpp"
#include <chrono>
#include <string>
namespace Domain {
using Date = std::chrono::year_month_day;
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
    Task(TaskID id,
        std::string title,
        std::string description, 
        Date deadline,
        SectionID section
    );
    ~Task() = default; // data members already know how free their owned resources
private:
    TaskID id;
    std::string title;
    std::string description;
    Date deadline;
    SectionID section;
};
}
