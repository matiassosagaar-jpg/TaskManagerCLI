#pragma once
#include <cstdint>
namespace Domain
{
template <typename IdType>
class IdGenerator {
public:
    IdType next();
    void init(uint64_t last_used);
private:
    uint64_t counter {0};
};
} // namespace Domain
