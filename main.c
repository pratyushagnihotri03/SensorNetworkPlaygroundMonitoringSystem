#include <stdio.h>
#include "contiki.h"
#include "rimeaddr.h"
#include "net/rime.h"
#include "vh400.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"

#define ID_SINK 201
#define ID_MOIST1 22998
#define ID_LIGHT1 0


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
	uint16_t id = from->u8[1] * 256 + from->u8[0];

	// test printf
	if (id == ID_LIGHT1) {
		printf("Light Value Received from node = %d.%d \n", from->u8[0], from->u8[1]);
		printf("light_value_TOTALSOLAR =    %u\n",p->value1); 
	}
	else if (id == ID_MOIST1) {
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
	PROCESS_BEGIN();

	static struct my_packet p;
	rimeaddr_t addr;
	addr.u8[0] = ID_SINK / 256;
	addr.u8[1] = ID_SINK % 256;
	uint16_t id = rimeaddr_node_addr.u8[1] * 256 + rimeaddr_node_addr.u8[0];

	if (id == ID_MOIST1)
  		SENSORS_ACTIVATE(vh400);
	else if (id == ID_LIGHT1)
		SENSORS_ACTIVATE(light_sensor);

	unicast_open(&uc, 140, &unicast_callbacks);

	if (id == ID_SINK) {
		// add something smart here
		while(1);
	}

   while(1) {
		etimer_set(&et, CLOCK_SECOND * 5);
		PROCESS_WAIT_UNTIL(etimer_expired(&et));

		if (id == ID_MOIST1) {
			p.value1 = vh400.value(ADC0);
			printf("id=%4u moisture=%u\n", id, p.value1);
		}
		else if (id == ID_LIGHT1) {
			p.value1 = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
			printf("light_value_TOTALSOLAR my sensor = %u\n",p.value1);
		}

		packetbuf_copyfrom(&p,sizeof(struct my_packet));
		unicast_send(&uc, &addr);
    }
    PROCESS_END();
}
