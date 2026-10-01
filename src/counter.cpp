#include "counter.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace practica
{

BcdCounter::BcdCounter(uint8_t initial_value)
    : value_(initial_value % 10U)
{
}

void BcdCounter::step(CountDirection direction)
{
    if (direction == CountDirection::UP)
    {
        value_ = (value_ == 9U)
                     ? 0U
                     : static_cast<uint8_t>(value_ + 1U);
    }
    else
    {
        value_ = (value_ == 0U)
                     ? 9U
                     : static_cast<uint8_t>(value_ - 1U);
    }
}

uint8_t BcdCounter::getValue() const
{
    return value_;
}

void BcdCounter::setValue(uint8_t value)
{
    value_ = value % 10U;
}

void counter_task(void *pvParameters)
{
    CounterConfig *config =
        static_cast<CounterConfig *>(pvParameters);

    if (config == nullptr)
    {
        vTaskDelete(nullptr);
        return;
    }

    BcdCounter counter(config->value);

    for (;;)
    {
        counter.setValue(config->value);

        counter.step(config->direction);

        config->value = counter.getValue();

        vTaskDelay(
            pdMS_TO_TICKS(config->period_ms)
        );
    }
}

} // namespace practica