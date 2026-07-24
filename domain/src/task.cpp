#include "task.hpp"

namespace Domain 
{
Task::Task(
        TaskID id,
        std::string title,
        std::string description,
        Priority priority,
        OptionalDate deadline,
        SectionID section
    ) : id{id}, title{title}, description{description},
        priority{priority}, deadline{deadline}, section{section} {}

} // namespace Domain