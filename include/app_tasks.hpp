#ifndef APP_TASKS_HPP
#define APP_TASKS_HPP

#include "system_state.hpp"

namespace practica
{

void task_manager(void *pvParameters);
void create_application_tasks(AppContext *context);

} 

#endif