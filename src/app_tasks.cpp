#include "app_tasks.hpp"
#include "app_config.hpp"
#include "buttons.hpp"
#include "counter.hpp"
#include "display.hpp"

#include "esp_log.h"

namespace practica
{

static const char *TAG = "TASK_MANAGER";

static CountDirection opposite_direction(
    CountDirection direction
)
{
    return (direction == CountDirection::UP)
               ? CountDirection::DOWN
               : CountDirection::UP;
}

static void apply_configuration(AppContext *c)
{
    c->counter1.direction =
        c->system.master_direction;

    if (c->system.coupling_mode ==
        CouplingMode::SAME)
    {
        c->counter2.direction =
            c->system.master_direction;
    }
    else
    {
        c->counter2.direction =
            opposite_direction(
                c->system.master_direction
            );
    }

    uint32_t period =
        (c->system.speed_mode ==
         SpeedMode::FAST)
            ? FAST_PERIOD_MS
            : SLOW_PERIOD_MS;

    c->counter1.period_ms = period;
    c->counter2.period_ms = period;
}

static void set_counters_running(
    AppContext *c,
    bool running
)
{
    if (running)
    {
        vTaskResume(c->counter1_handle);
        vTaskResume(c->counter2_handle);
    }
    else
    {
        vTaskSuspend(c->counter1_handle);
        vTaskSuspend(c->counter2_handle);
    }
}

static void synchronize_for_same(
    AppContext *c
)
{
    uint8_t value1 = c->counter1.value;
    uint8_t value2 = c->counter2.value;

    uint8_t selected;

    if (c->system.master_direction ==
        CountDirection::UP)
    {
        selected =
            (value1 > value2)
                ? value1
                : value2;
    }
    else
    {
        selected =
            (value1 < value2)
                ? value1
                : value2;
    }

    c->counter1.value = selected;
    c->counter2.value = selected;
}

void task_manager(void *pvParameters)
{
    AppContext *c =
        static_cast<AppContext *>(pvParameters);

    if (c == nullptr)
    {
        vTaskDelete(nullptr);
        return;
    }

    apply_configuration(c);

    // El Task Manager controla la pausa inicial.
    set_counters_running(c, false);

    // Restaurar valores iniciales por si alguna tarea
    // alcanzó a ejecutar una vez antes de ser suspendida.
    c->counter1.value = 0U;
    c->counter2.value = 9U;

    ESP_LOGI(
        TAG,
        "STATE=%s",
        run_state_to_string(
            c->system.run_state
        )
    );

    ESP_LOGI(
        TAG,
        "DIR=%s",
        direction_to_string(
            c->system.master_direction
        )
    );

    ESP_LOGI(
        TAG,
        "SPEED=%s",
        speed_to_string(
            c->system.speed_mode
        )
    );

    ESP_LOGI(
        TAG,
        "MODE=%s",
        coupling_to_string(
            c->system.coupling_mode
        )
    );

    for (;;)
    {
        if (c->start_pause_event.pending)
        {
            c->start_pause_event.pending = false;

            c->system.run_state =
                (c->system.run_state ==
                 RunState::PAUSED)
                    ? RunState::RUNNING
                    : RunState::PAUSED;

            set_counters_running(
                c,
                c->system.run_state ==
                    RunState::RUNNING
            );

            ESP_LOGI(
                TAG,
                "STATE=%s",
                run_state_to_string(
                    c->system.run_state
                )
            );
        }

        if (c->system.run_state ==
            RunState::RUNNING)
        {
            if (c->direction_event.pending)
            {
                c->direction_event.pending = false;

                c->system.master_direction =
                    (c->system.master_direction ==
                     CountDirection::UP)
                        ? CountDirection::DOWN
                        : CountDirection::UP;

                apply_configuration(c);

                ESP_LOGI(
                    TAG,
                    "DIR=%s",
                    direction_to_string(
                        c->system.master_direction
                    )
                );
            }

            if (c->speed_event.pending)
            {
                c->speed_event.pending = false;

                c->system.speed_mode =
                    (c->system.speed_mode ==
                     SpeedMode::SLOW)
                        ? SpeedMode::FAST
                        : SpeedMode::SLOW;

                apply_configuration(c);

                ESP_LOGI(
                    TAG,
                    "SPEED=%s",
                    speed_to_string(
                        c->system.speed_mode
                    )
                );
            }

            if (c->mode_event.pending)
            {
                c->mode_event.pending = false;

                if (c->system.coupling_mode ==
                    CouplingMode::OPPOSITE)
                {
                    set_counters_running(c, false);

                    synchronize_for_same(c);

                    c->system.coupling_mode =
                        CouplingMode::SAME;

                    apply_configuration(c);

                    set_counters_running(c, true);
                }
                else
                {
                    c->system.coupling_mode =
                        CouplingMode::OPPOSITE;

                    apply_configuration(c);
                }

                ESP_LOGI(
                    TAG,
                    "MODE=%s",
                    coupling_to_string(
                        c->system.coupling_mode
                    )
                );
            }
        }
        else
        {
            c->direction_event.pending = false;
            c->speed_event.pending = false;
            c->mode_event.pending = false;
        }

        vTaskDelay(
            pdMS_TO_TICKS(
                MANAGER_PERIOD_MS
            )
        );
    }
}

void create_application_tasks(
    AppContext *c
)
{
    if (c == nullptr)
    {
        return;
    }

    static ButtonTaskParams start_params;
    static ButtonTaskParams direction_params;
    static ButtonTaskParams speed_params;
    static ButtonTaskParams mode_params;

    start_params =
    {
        BTN_START_PAUSE,
        &c->start_pause_event
    };

    direction_params =
    {
        BTN_DIRECTION,
        &c->direction_event
    };

    speed_params =
    {
        BTN_SPEED,
        &c->speed_event
    };

    mode_params =
    {
        BTN_MODE,
        &c->mode_event
    };

    xTaskCreate(
        counter_task,
        "Counter1",
        2048,
        &c->counter1,
        2,
        &c->counter1_handle
    );

    xTaskCreate(
        counter_task,
        "Counter2",
        2048,
        &c->counter2,
        2,
        &c->counter2_handle
    );

    /*
     * Se crea inmediatamente el TaskManager con
     * prioridad mayor para que tome control de
     * Counter1 y Counter2.
     */
    xTaskCreate(
        task_manager,
        "TaskManager",
        3072,
        c,
        3,
        &c->manager_handle
    );

    xTaskCreate(
        button_task,
        "BtnStart",
        2048,
        &start_params,
        2,
        nullptr
    );

    xTaskCreate(
        button_task,
        "BtnDirection",
        2048,
        &direction_params,
        2,
        nullptr
    );

    xTaskCreate(
        button_task,
        "BtnSpeed",
        2048,
        &speed_params,
        2,
        nullptr
    );

    xTaskCreate(
        button_task,
        "BtnMode",
        2048,
        &mode_params,
        2,
        nullptr
    );

    xTaskCreate(
        display_refresh_task,
        "DisplayRefresh",
        2048,
        c,
        2,
        nullptr
    );
}

} // namespace practica