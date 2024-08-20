#include "contiki.h"
#include "sys/ctimer.h"
#include <stdio.h>

/* Declaración del proceso */
PROCESS(ctimer_example_process, "ctimer Example Process");
AUTOSTART_PROCESSES(&ctimer_example_process);

/* Función de callback para ctimer */
void ctimer_callback(void *ptr) {
    printf("ctimer expirado! Ejecutando la función de callback.\n");
}

/* Hilo del proceso */
PROCESS_THREAD(ctimer_example_process, ev, data) {
    /* Variable para el ctimer */
    static struct ctimer my_ctimer;

    PROCESS_BEGIN();

    /* Configuración del ctimer para expirar en 5 segundos */
    ctimer_set(&my_ctimer, CLOCK_SECOND * 5, ctimer_callback, NULL);

    /* Espera hasta que el ctimer expire */
    PROCESS_WAIT_EVENT_UNTIL(ctimer_expired(&my_ctimer));

    printf("ctimer ha terminado de ejecutarse.\n");

    PROCESS_END();
}