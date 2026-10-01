#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <cstdint>

#include "system_state.hpp"

namespace practica
{

class BcdCounter
{
public:
    explicit BcdCounter(uint8_t initial_value = 0U);

    void step(CountDirection direction);
    uint8_t getValue() const;
    void setValue(uint8_t value);

private:
    uint8_t value_;
};

void counter_task(void *pvParameters);

} // namespace practica

#endif