#pragma once
#include <cstdint>
#include "ids.hpp"
namespace Domain
{
template <ID IdType>
class IdGenerator {
public:
    IdType next() {return IdType{++counter};}
    void set_last_used(uint64_t last_used) {counter = last_used;}
private:
    uint64_t counter {0};
};

} // namespace Domain
