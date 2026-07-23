#pragma once
#include <cstdint>
#include "ids.hpp"
#include "section.hpp"
#include "task.hpp"
#include "id_generator.hpp"
namespace Domain {
class TaskManager {
public:
    //------------Interface--------------
private:
    IdGenerator<TaskID> task_ids;
    IdGenerator<SectionID> section_ids;
};
} // namespace Domain