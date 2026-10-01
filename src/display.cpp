#include "display.hpp"

#include "app_config.hpp"
#include "system_state.hpp"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace practica
{

static const uint8_t DIGITS[10] =
{
    0x3FU,
    0x06U,
    0x5BU,
    0x4FU,
    0x66U,
    0x6DU,
    0x7DU,
    0x07U,
    0x7FU,
    0x6FU
};

static const gpio_num_t SEGMENTS[7] =
{
    SEG_A,
    SEG_B,
    SEG_C,
    SEG_D,
    SEG_E,
    SEG_F,
    SEG_G
};

void SevenSegmentDisplay::init()
{
    for (uint8_t i = 0U; i < 7U; ++i)
    {
        gpio_reset_pin(SEGMENTS[i]);

        gpio_set_direction(
            SEGMENTS[i],
            GPIO_MODE_OUTPUT
        );
    }

    gpio_reset_pin(DISPLAY_1_EN);

    gpio_set_direction(
        DISPLAY_1_EN,
        GPIO_MODE_OUTPUT
    );

    gpio_reset_pin(DISPLAY_2_EN);

    gpio_set_direction(
        DISPLAY_2_EN,
        GPIO_MODE_OUTPUT
    );

    blankAll();

    writeSegments(0U);
}

void SevenSegmentDisplay::writeSegments(uint8_t pattern)
{
    for (uint8_t i = 0U; i < 7U; ++i)
    {
        gpio_set_level(
            SEGMENTS[i],
            (pattern >> i) & 0x01U
        );
    }
}

void SevenSegmentDisplay::blankAll()
{
    gpio_set_level(
        DISPLAY_1_EN,
        DISPLAY_OFF_LEVEL
    );

    gpio_set_level(
        DISPLAY_2_EN,
        DISPLAY_OFF_LEVEL
    );
}

void SevenSegmentDisplay::showDigit(
    uint8_t display_id,
    uint8_t digit
)
{
    if (digit > 9U)
    {
        return;
    }

    blankAll();

    writeSegments(
        DIGITS[digit]
    );

    if (display_id == 1U)
    {
        gpio_set_level(
            DISPLAY_1_EN,
            DISPLAY_ON_LEVEL
        );
    }
    else if (display_id == 2U)
    {
        gpio_set_level(
            DISPLAY_2_EN,
            DISPLAY_ON_LEVEL
        );
    }
}

void SevenSegmentDisplay::blank(uint8_t display_id)
{
    if (display_id == 1U)
    {
        gpio_set_level(
            DISPLAY_1_EN,
            DISPLAY_OFF_LEVEL
        );
    }
    else if (display_id == 2U)
    {
        gpio_set_level(
            DISPLAY_2_EN,
            DISPLAY_OFF_LEVEL
        );
    }
}

void display_refresh_task(void *pvParameters)
{
    AppContext *context =
        static_cast<AppContext *>(pvParameters);

    if (context == nullptr)
    {
        vTaskDelete(nullptr);
        return;
    }

    SevenSegmentDisplay display;

    display.init();

    for (;;)
    {
        display.showDigit(
            1U,
            context->counter1.value
        );

        vTaskDelay(
            pdMS_TO_TICKS(
                DISPLAY_REFRESH_MS
            )
        );

        display.showDigit(
            2U,
            context->counter2.value
        );

        vTaskDelay(
            pdMS_TO_TICKS(
                DISPLAY_REFRESH_MS
            )
        );
    }
}

} // namespace practica