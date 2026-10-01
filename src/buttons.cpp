#include "buttons.hpp"
#include "app_config.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace practica
{

void button_init(gpio_num_t pin)
{
    gpio_reset_pin(pin);

    gpio_set_direction(
        pin,
        GPIO_MODE_INPUT
    );

    gpio_set_pull_mode(
        pin,
        GPIO_PULLUP_ONLY
    );
}

bool button_is_pressed(gpio_num_t pin)
{
    return gpio_get_level(pin) == 0;
}

void button_task(void *pvParameters)
{
    ButtonTaskParams *params =
        static_cast<ButtonTaskParams *>(pvParameters);

    if ((params == nullptr) ||
        (params->event == nullptr))
    {
        vTaskDelete(nullptr);
        return;
    }

    button_init(params->pin);

    bool previous =
        button_is_pressed(params->pin);

    for (;;)
    {
        const bool current =
            button_is_pressed(params->pin);

        if (current && !previous)
        {
            params->event->pending = true;
        }

        previous = current;

        vTaskDelay(
            pdMS_TO_TICKS(
                BUTTON_PERIOD_MS
            )
        );
    }
}

} // namespace practica