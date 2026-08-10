    #pragma once
#include <cstdint>
#include "ids.hpp"
#include "state_report.hpp"
namespace Domain
{
template <ID IdType>
class IDGenerator {
public:
    IdType next() {return IdType{++counter};}
    void set_last_used(uint64_t last_used) {counter = last_used;}
    State::IDGeneratorReport report_state() const {return {counter};}
    private:
    uint64_t counter {0};
};

} // namespace Domain
