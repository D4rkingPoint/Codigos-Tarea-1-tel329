#include "contiki.h"
#include <stdio.h>

PROCESS(sensor_process, "Sensor Process");
AUTOSTART_PROCESSES(&sensor_process);

static void collect_temperature_data() {
    // Simular la recoleccion de datos de temperatura
    printf("Datos de temperatura recolectados.\n");
}

PROCESS_THREAD(sensor_process, ev, data)
{
    PROCESS_BEGIN();

    while (1) {
        // Recoleccion de datos de temperatura
        collect_temperature_data();

        // Ceder el control para permitir que otros procesos se ejecuten
        PROCESS_YIELD();
    }

    PROCESS_END();
}
