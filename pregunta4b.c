#include "contiki.h"
#include "sys/etimer.h"
#include <stdio.h>

/* Declaración del proceso */
PROCESS(etimer_example_process, "etimer Example Process");
AUTOSTART_PROCESSES(&etimer_example_process);

/* Hilo del proceso */
PROCESS_THREAD(etimer_example_process, ev, data) {
    /* Variable para el etimer */
    static struct etimer my_etimer;

    PROCESS_BEGIN();

    /* Configuración del etimer para expirar en 10 segundos */
    etimer_set(&my_etimer, CLOCK_SECOND * 10);

    /* Espera hasta que el etimer expire */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&my_etimer));

    printf("etimer expirado! Procesando evento del etimer.\n");

    PROCESS_END();
}