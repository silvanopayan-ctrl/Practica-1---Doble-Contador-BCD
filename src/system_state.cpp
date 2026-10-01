#include "system_state.hpp"
#include "app_config.hpp"

namespace practica
{

void system_state_init(AppContext *c)
{
    if (c == nullptr)
    {
        return;
    }

    c->system.run_state = RunState::PAUSED;
    c->system.master_direction = CountDirection::UP;
    c->system.coupling_mode = CouplingMode::OPPOSITE;
    c->system.speed_mode = SpeedMode::SLOW;

    c->counter1.value = 0U;
    c->counter1.direction = CountDirection::UP;
    c->counter1.period_ms = SLOW_PERIOD_MS;
    c->counter1.display_id = 1U;

    c->counter2.value = 9U;
    c->counter2.direction = CountDirection::DOWN;
    c->counter2.period_ms = SLOW_PERIOD_MS;
    c->counter2.display_id = 2U;

    c->start_pause_event.pending = false;
    c->start_pause_event.id = ButtonId::START_PAUSE;

    c->direction_event.pending = false;
    c->direction_event.id = ButtonId::DIRECTION;

    c->speed_event.pending = false;
    c->speed_event.id = ButtonId::SPEED;

    c->mode_event.pending = false;
    c->mode_event.id = ButtonId::MODE;

    c->counter1_handle = nullptr;
    c->counter2_handle = nullptr;
    c->manager_handle = nullptr;
}

const char *direction_to_string(CountDirection d)
{
    return (d == CountDirection::UP) ? "UP" : "DOWN";
}

const char *coupling_to_string(CouplingMode m)
{
    return (m == CouplingMode::SAME) ? "SAME" : "OPPOSITE";
}

const char *run_state_to_string(RunState s)
{
    return (s == RunState::RUNNING) ? "RUNNING" : "PAUSED";
}

const char *speed_to_string(SpeedMode s)
{
    return (s == SpeedMode::FAST) ? "FAST" : "SLOW";
}

} // namespace practica