#include "domain.hpp"
#include "serialization.hpp"
#include "file_handler.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
namespace TestCases 
{
using namespace Domain;
using namespace Persistence;
// Manager Examples
TaskManager empty_state_manager() {return TaskManager();}
TaskManager one_empty_section() {
    TaskManager manager;
    manager.create_section("Programming");
    return manager;
}
TaskManager one_task_without_section() {
    TaskManager manager;
    manager.create_task(
        "NoSectionTask",
        "description here",
        Priority::Urgent,
        std::nullopt,
        SectionID::none()
    );
    return manager;
}
TaskManager section_with_task() {
    TaskManager manager;
    SectionID section_id = manager.create_section("IHaveATask");
    manager.create_task(
        "Task1",
        "I have a section",
        Priority::Medium,
        Date{std::chrono::year{2026},
          std::chrono::month{8},
          std::chrono::day{20}},
          section_id
    );
    return manager;
}
TaskManager some_sections_with_tasks() {
    TaskManager manager;
    SectionID section_id = manager.create_section("Got A lot of tasks in me");
    manager.create_task(
        "Task1",
        "I am the first task",
        Priority::Low,
        Date{std::chrono::year{2026},
          std::chrono::month{8},
          std::chrono::day{21}},
          section_id
    );    
    manager.create_task(
        "Task2",
        "I am the second task",
        Priority::Medium,
        Date{std::chrono::year{2026},
          std::chrono::month{8},
          std::chrono::day{22}},
          section_id
    );
    manager.create_task(
        "Task3",
        "I am the third task",
        Priority::High,
        Date{std::chrono::year{2026},
          std::chrono::month{8},
          std::chrono::day{23}},
          section_id
    );

    return manager;
}
TaskManager multiple_sections_and_tasks() {
    TaskManager manager;
    SectionID section1_id = manager.create_section("Section 1");
    manager.create_task(
        "Task1",
        "I am the first task",
        Priority::Low,
        Date{std::chrono::year{2026},
          std::chrono::month{8},
          std::chrono::day{21}},
          section1_id
    );    
    manager.create_task(
        "Task2",
        "I am the second task",
        Priority::Medium,
        Date{std::chrono::year{2026},
          std::chrono::month{8},
          std::chrono::day{22}},
          section1_id
    );

    SectionID section2_id = manager.create_section("Section 2");
    manager.create_task(
        "Task3",
        "I am the third task",
        Priority::High,
        Date{std::chrono::year{2026},
          std::chrono::month{8},
          std::chrono::day{23}},
          section2_id
    );

    manager.create_section("Section 3");
    return manager;
}

void integration_test(std::string test_name, TaskManager manager) {
    using namespace Domain;
    using namespace Persistence;
    using json = nlohmann::json;
    // Start a manager and do stuff to it    
    //report state
    State::ManagerFullReport original_report = manager.report_state();

    //turn to json
    json original_data = Adapter::to_json(original_report);
    //save data to filesystem
    FileHandler::save(original_data);
    //retrieve saved data:
    json retrieved_data = FileHandler::load();
    State::ManagerFullReport retrieved_report = Adapter::full_report_from_json(retrieved_data);
    //load state into a new manager
    TaskManager second_manager;
    second_manager.load_state(retrieved_report);
    


    //compare them
    if (manager.report_state() == second_manager.report_state())
        std::cout << test_name << "Test passed\n";
    else
        std::cout << test_name << "Test failed\n";
}
} // namespace TestCases 
int main() {
    using namespace TestCases;
    integration_test("empty_state_manager", empty_state_manager());
    integration_test("one_empty_section",one_empty_section());
    integration_test("one_task_without_section",one_task_without_section());
    integration_test("section_with_task",section_with_task());
    integration_test("some_sections_with_tasks",some_sections_with_tasks());
    integration_test("multiple_sections_and_tasks",multiple_sections_and_tasks());
}