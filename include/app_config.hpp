#ifndef APP_CONFIG_HPP
#define APP_CONFIG_HPP

#include <cstdint>
#include "driver/gpio.h"

namespace practica
{

constexpr gpio_num_t SEG_A = GPIO_NUM_13;
constexpr gpio_num_t SEG_B = GPIO_NUM_12;
constexpr gpio_num_t SEG_C = GPIO_NUM_14;
constexpr gpio_num_t SEG_D = GPIO_NUM_27;
constexpr gpio_num_t SEG_E = GPIO_NUM_26;
constexpr gpio_num_t SEG_F = GPIO_NUM_25;
constexpr gpio_num_t SEG_G = GPIO_NUM_33;

constexpr gpio_num_t DISPLAY_1_EN = GPIO_NUM_32;
constexpr gpio_num_t DISPLAY_2_EN = GPIO_NUM_23;

constexpr gpio_num_t BTN_START_PAUSE = GPIO_NUM_18;
constexpr gpio_num_t BTN_DIRECTION   = GPIO_NUM_19;
constexpr gpio_num_t BTN_SPEED       = GPIO_NUM_21;
constexpr gpio_num_t BTN_MODE        = GPIO_NUM_22;

constexpr uint32_t SLOW_PERIOD_MS = 500U;
constexpr uint32_t FAST_PERIOD_MS = 250U;
constexpr uint32_t BUTTON_PERIOD_MS = 20U;
constexpr uint32_t MANAGER_PERIOD_MS = 10U;
constexpr uint32_t DISPLAY_REFRESH_MS = 2U;

constexpr uint32_t DISPLAY_ON_LEVEL = 0U;
constexpr uint32_t DISPLAY_OFF_LEVEL = 1U;

} // namespace practica

#endif