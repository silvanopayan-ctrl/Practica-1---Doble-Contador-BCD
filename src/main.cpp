
/*
#include "app_tasks.hpp"
#include "system_state.hpp"

extern "C" void app_main(void)
{
    static practica::AppContext context;

    practica::system_state_init(
        &context
    );

    practica::create_application_tasks(
        &context
    );
}

*/

#include "app_tasks.hpp"
#include "system_state.hpp"

extern "C" void app_main(void)
{
    static practica::AppContext context;

    practica::system_state_init(&context);
    practica::create_application_tasks(&context);
} 