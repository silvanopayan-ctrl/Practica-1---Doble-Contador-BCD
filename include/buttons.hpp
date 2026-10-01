#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include "driver/gpio.h"
#include "system_state.hpp"

namespace practica
{

struct ButtonTaskParams
{
    gpio_num_t pin;
    ButtonEvent *event;
};

void button_init(gpio_num_t pin);
bool button_is_pressed(gpio_num_t pin);
void button_task(void *pvParameters);

} // namespace practica

#endif