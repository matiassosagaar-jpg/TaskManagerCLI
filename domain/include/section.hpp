#pragma once
#include "ids.hpp"
#include <vector>
#include <string>
namespace Domain {
class Section {
public:
    // ----------Interface-------------
    Section(std::string name, SectionID id);
private:
    std::string name;
    SectionID id;
    std::vector<TaskID> tasks;

};
}