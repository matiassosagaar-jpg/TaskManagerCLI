#pragma once
#include "state_report.hpp"
#include "json_schema.hpp"
#include "persistence_helpers.hpp"
#include <chrono>
#include <sstream>
namespace Persistence {
using namespace Schema;
    JsonSchema task_schema();
    JsonSchema section_schema();
    JsonSchema manager_schema();
}; // namespace Saver
