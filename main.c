#include <stdio.h>
#include "contiki.h"
#include "rimeaddr.h"
#include "vh400.h"

#define ID_MOIST1 22998

PROCESS(main_process, "main");
AUTOSTART_PROCESSES(&main_process);

static struct etimer et;

PROCESS_THREAD(main_process, ev, data)
{
        PROCESS_BEGIN();
	uint16_t id = rimeaddr_node_addr.u8[1] * 256 + rimeaddr_node_addr.u8[0];
	
	if (id == ID_MOIST1)
  		SENSORS_ACTIVATE(vh400);

        while(1) {
		if (id == ID_MOIST1) {
			etimer_set(&et, CLOCK_SECOND * 2);
    			PROCESS_WAIT_UNTIL(etimer_expired(&et));
			printf("id=%4u moisture=%u\n", id,vh400.value(ADC0));
		}
        }
        PROCESS_END();
}
