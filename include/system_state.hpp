#ifndef SYSTEM_STATE_HPP
#define SYSTEM_STATE_HPP

#include <cstdint>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace practica
{

enum class CountDirection
{
    UP,
    DOWN
};

enum class CouplingMode
{
    OPPOSITE,
    SAME
};

enum class RunState
{
    PAUSED,
    RUNNING
};

enum class SpeedMode
{
    SLOW,
    FAST
};

enum class ButtonId
{
    START_PAUSE,
    DIRECTION,
    SPEED,
    MODE
};

struct CounterConfig
{
    volatile uint8_t value;
    volatile CountDirection direction;
    volatile uint32_t period_ms;
    uint8_t display_id;
};

struct ButtonEvent
{
    volatile bool pending;
    ButtonId id;
};

struct SystemState
{
    volatile RunState run_state;
    volatile CountDirection master_direction;
    volatile CouplingMode coupling_mode;
    volatile SpeedMode speed_mode;
};

struct AppContext
{
    SystemState system;

    CounterConfig counter1;
    CounterConfig counter2;

    ButtonEvent start_pause_event;
    ButtonEvent direction_event;
    ButtonEvent speed_event;
    ButtonEvent mode_event;

    TaskHandle_t counter1_handle;
    TaskHandle_t counter2_handle;
    TaskHandle_t manager_handle;
};

void system_state_init(AppContext *context);

const char *direction_to_string(CountDirection direction);
const char *coupling_to_string(CouplingMode mode);
const char *run_state_to_string(RunState state);
const char *speed_to_string(SpeedMode speed);

} // namespace practica

#endif