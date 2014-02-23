#include <stdio.h>
#include "contiki.h"
#include "rimeaddr.h"
#include "net/rime.h"
#include "vh400.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"
#include "settings.h"

/* unicast packet ptructure */ 
struct my_packet{
	uint16_t  value1;
	uint16_t  value2;
};

/* Unicast Receive Function */
static void
recv(struct unicast_conn *c, const rimeaddr_t *from)
{
	struct my_packet * p;

	p = (struct my_packet *)packetbuf_dataptr(); /* Cast Packet */
	uint16_t from_id = from->u8[1] * 256 + from->u8[0];

	// test printf
	if (from_id == ID_LIGHT1) {
		printf("Light Valueis from node = %d.%d are %u and %u\n", from->u8[0], from->u8[1], p->value1, p->value2);
	}
	else if (from_id == ID_MOIST1) {
		printf("moisture value from node %d.%d is %u\n", from->u8[0], from->u8[1], p->value1);
	}
}
/* Unicast Connection */
static const struct unicast_callbacks unicast_callbacks = {recv};
static struct unicast_conn uc;
/*---------------------------------------------------------------------------*/


PROCESS(main_process, "main");
AUTOSTART_PROCESSES(&main_process);

static struct etimer et;

PROCESS_THREAD(main_process, ev, data)
{
	static struct my_packet p;
	static rimeaddr_t addr;
	static uint16_t my_id;

	PROCESS_EXITHANDLER(unicast_close(&uc);)
	PROCESS_BEGIN();

	my_id = rimeaddr_node_addr.u8[1] * 256 + rimeaddr_node_addr.u8[0];
	addr.u8[0] = ID_SINK % 256;
	addr.u8[1] = ID_SINK / 256;

	if (my_id == ID_MOIST1)
  		SENSORS_ACTIVATE(vh400);
	else if (my_id == ID_LIGHT1)
		SENSORS_ACTIVATE(light_sensor);

	unicast_open(&uc, 140, &unicast_callbacks);

	if (my_id == ID_SINK) {
		// start actuators
		printf(”PG:START\n”);

		// add something smart here
		while(1) {
			PROCESS_YIELD();
 		}

		// stop actuators
		printf(”PG:ENG\n”);
	}

   while(1) {
		etimer_set(&et, CLOCK_SECOND * 5);
		PROCESS_WAIT_UNTIL(etimer_expired(&et));

		if (my_id == ID_MOIST1) {
			p.value1 = vh400.value(ADC0);
			printf("my_id=%u moisture=%u\n", my_id, p.value1);
		}
		else if (my_id == ID_LIGHT1) {
			p.value1 = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
			p.value2 = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
			printf("my_id=%u total_solar=%u photosynthetic=%u\n",my_id, p.value1, p.value2);
		}

		packetbuf_copyfrom(&p,sizeof(struct my_packet));

		unicast_send(&uc, &addr);	
    }
    PROCESS_END();
}
