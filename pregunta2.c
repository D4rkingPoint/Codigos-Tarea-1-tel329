#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "dev/leds.h"

#include <string.h>
#include <stdio.h> /* For printf() */

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define INITIAL_INTERVAL (2 * CLOCK_SECOND) // Valor inicial del intervalo
#define MAX_INTERVAL (20 * CLOCK_SECOND)    // Intervalo maximo
#define INTERVAL_STEP (2 * CLOCK_SECOND)   // Incremento del intervalo
#define LED_ON_TIME (1 * CLOCK_SECOND)     // Tiempo durante el cual el LED rojo estara encendido
#define MAX_SENSORS 10

static linkaddr_t dest_addr = {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr = {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#endif /* MAC_CONF_WITH_TSCH */

/*---------------------------------------------------------------------------*/
PROCESS(Unicast_led, "NullNet unicast example");
AUTOSTART_PROCESSES(&Unicast_led);

/*---------------------------------------------------------------------------*/
/* Input callback function */
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
  if(len == sizeof(unsigned)) {
    unsigned count;
    memcpy(&count, data, sizeof(count));
    leds_on(LEDS_GREEN);
    LOG_INFO("El Sensor ");
    LOG_INFO_LLADDR(dest);
    LOG_INFO_(" recibio el paquete %u\n", count);
    //etimer_set(&periodic_timer, CLOCK_SECOND); //espera 1 segundo
    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    //leds_off(LEDS_GREEN);
  }
}

/*---------------------------------------------------------------------------*/
/* Main process */
PROCESS_THREAD(Unicast_led, ev, data)
{
  static struct etimer periodic_timer;
  static struct etimer led_timer;
  static unsigned count = 0;
  static clock_time_t current_interval = INITIAL_INTERVAL;

  PROCESS_BEGIN();

#if MAC_CONF_WITH_TSCH
  tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
#endif /* MAC_CONF_WITH_TSCH */

  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&count;
  nullnet_len = sizeof(count);
  nullnet_set_input_callback(input_callback);

  if(!linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) {
    etimer_set(&periodic_timer, current_interval);
    while(1) {
      
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

      leds_on(LEDS_RED); // Encender LED rojo para transmitir
      etimer_set(&led_timer, LED_ON_TIME); // Temporizador para mantener el LED rojo encendido
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&led_timer));
      leds_off(LEDS_RED); // Apagar el LED rojo
      
      LOG_INFO("Enviado el paquete %u a ", count);
      LOG_INFO_LLADDR(&dest_addr);
      LOG_INFO_("\n");
      
      NETSTACK_NETWORK.output(&dest_addr);
      count++;
      
      // Incrementar el intervalo
      current_interval += INTERVAL_STEP;
      if(current_interval > MAX_INTERVAL) {
        current_interval = INITIAL_INTERVAL;
      }
      
      etimer_set(&periodic_timer, current_interval);
    }
  }

  PROCESS_END();
}