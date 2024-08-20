EVENT UNTIL }, label={lst:pregunta3a}]

#include "contiki.h"
#include <stdio.h>

#define DATA_COLLECTION_INTERVAL (CLOCK_SECOND * 10)  // Intervalo de 10 segundos

PROCESS(sensor_process, "Sensor Process");
AUTOSTART_PROCESSES(&sensor_process);

static struct etimer timer;

static void collect_temperature_data() {
    // Simular la recoleccion de datos de temperatura
    printf("Datos de temperatura recolectados.\n");
}

PROCESS_THREAD(sensor_process, ev, data)
{
    PROCESS_BEGIN();

    // Inicializar el temporizador
    etimer_set(&timer, DATA_COLLECTION_INTERVAL);

    while (1) {
        // Esperar a que expire el temporizador
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (ev == PROCESS_EVENT_TIMER) {
            // Recoleccion de datos de temperatura
            collect_temperature_data();

            // Reiniciar el temporizador
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}