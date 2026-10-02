Practica 1 - Doble-Contador BCD

INTEGRANTES---------------------------------------------------------------
Patricio Mata Villanueva - 10110
Silvano Payan Covarrubias - 10070
Mario Alberto Gonzalez Ramirez - 10107

CONCLUSIONES PERSONALES---------------------------------------------------
Patricio Mata Villanueva
Al momento de realizar la práctica me pude percatar de todos los errores puestos a propósito para despistarnos, aunque ingeniosas, se veían muy fácilmente pues la estructura del código era muy limpia, tanto declarando tareas como funciones, lo complicado y lo que dejé pasar fue al último, a nivel de bits, no vi que siempre había overflows y por lo tanto no funcionaba el código, aunque no tuviera errores, en conclusión siempre hay que poner atención en que estamos haciendo y declarando.

Silvano Payan Covarrubias
Esta práctica me permitió comprender mejor cómo funciona FreeRTOS con el uso de pvParameters y TaskHandle_t. También entendí la importancia de separar las responsabilidades del programa. La funcion mas util que aprendi fue enum class me parece óptimo el clasificar los estados mediante sus nombres que uno pueda definir. Finalmente, pude identificar que al trabajar con información compartida entre tareas existen riesgos de concurrencia, los cuales posteriormente podrían controlarse mediante mecanismos de sincronización como queues o semáforos.

Mario Alberto Gonzalez Ramirez
En esta práctica se pudo reconocer la importancia de organizar un sistema con varias tareas en FreeRTOS y de reutilizar el código de manera eficiente. Un punto importante fue observar que una misma función puede utilizarse para crear diferentes tareas, como en los dos contadores, y que mediante pvParameters cada una puede recibir información distinta y trabajar como una instancia independiente. También se vio la utilidad de TaskHandle_t y del Task Manager, ya que este último se encarga de interpretar los eventos de los botones, actualizar la configuración del sistema y controlar la suspensión o reanudación de las tareas de los contadores. Además, la multiplexación de los displays permitió comprender cómo se pueden compartir las mismas líneas de segmentos y habilitar cada display por separado.

CUESTIONARIO-----------------------------------------------------------------
1. ¿Por qué Counter1 y Counter2 pueden ejecutar la misma función counterTask() y comportarse diferente?
Ambas tareas reciben diferentes parámetros mediante pvParameters

2. ¿Qué información recibe cada tarea mediante pvParameters?
En counter_task() se recibe la información: value, direction y period_ms

3. ¿Qué representa un TaskHandle_t y por qué el Task Manager necesita conservarlo?
El TaskHandle_t es un identificador que permite referirnos a una tarea especifica. Task Manager controla directamente las tareas Counter1 y Counter2, mediante vTaskSuspend() o vTaskResume. Por eso es necesario el TaskHandle_t para que TaskManager controle cada contador.

4. ¿Qué diferencia existe entre BLOCKED y SUSPENDED en esta práctica?
BLOCKED hace que la tarea espere a que ocurra algo o pase algun tiempo, en cambio, SUSPENDED detiene por completo la tarea, y solamente se pueda reanudar mediante otra tarea

5. ¿Qué ocurre con vTaskDelay() cuando una tarea es suspendida?
Cuando la tarea es suspendida no importa el vTaskDelay(), ya que este solamente establece un periodo en el cual la tarea se bloquea, pero si la tarea esta suspendida no surte efecto.

6. ¿Qué ventaja aporta enum class frente a constantes enteras para representar estados?
enum class permite representar los estados mediante nombres claros y con un tipo especifico. Asi cualquiera puede comprender para que es ese estado.

7. ¿Qué responsabilidad tiene BcdCounter y cuál SevenSegmentDisplay?
BcdCounter practicamente es el que se encarga de hacer la lógica para el conteo y SevenSegmentDisplay se encarga de que ese numerito sea vea bonito y correcto en el display fisico.

8. ¿Por qué volatile no resuelve por sí solo los problemas de concurrencia?
volatile solamente le dice al compilador que la variable cambia externamente, que no la contemple como constante. No proporciona ningun mecanismo de coordinacion ni nada.

9. ¿Qué cambiaría en el diseño cuando posteriormente se permitan queues o semáforos?
Se podría utilizar mecanismos de sincronización para controlar el acceso a la información compartida y comunicar eventos entre tareas.

10. Si ambas tareas tienen la misma prioridad, ¿cómo interviene el scheduler de FreeRTOS? 
El scheduler reparte el tiempo de CPU para que paresca que esta siendo ejecutado independientemente cada tarea.