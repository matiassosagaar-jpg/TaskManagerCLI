#include "domain.hpp"
using namespace Domain;

#include "domain.hpp"
#include <cassert>
#include <iostream>
#include  "json_schema.hpp"


int main() {
    // Task manager smoke test

    using namespace Domain;
    TaskManager manager{};

    // Creating section
    SectionID section_id = manager.create_section("Work");

    // Creating task
    TaskID task_id = manager.create_task(
        "Finish backend",
        "Implement persistence layer",
        Priority::High,
        std::nullopt,
        section_id
    );

    // Querying task
    const Task& task = manager.get_task(task_id);

    assert(task.get_title() == "Finish backend");
    assert(task.get_sectionID() == section_id);

    // Querying section
    const Section& section = manager.get_section(section_id);

    assert(section.contains_task(task_id));

    // Removing task
    manager.remove_task(task_id);

    assert(!manager.get_section(section_id).contains_task(task_id));

    // Removing empty section
    manager.remove_section(section_id);

    std::cout << "Domain smoke test passed!\n";
    
    
}