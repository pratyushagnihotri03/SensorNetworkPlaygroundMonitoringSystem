#include <stdio.h>
#include "contiki.h"
#include "rimeaddr.h"
#include "net/rime.h"
#include "vh400.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"
#include "dev/sht11-sensor.h"
#include "ds1000-sensor.h"
#include "settings.h"

/* unicast packet ptructure */ 
struct my_packet{
	uint16_t  value1;
	uint16_t  value2;
	uint16_t	 value3;
};

/* Unicast Receive Function */
static void
recv(struct unicast_conn *c, const rimeaddr_t *from)
{
	struct my_packet * p;

	p = (struct my_packet *)packetbuf_dataptr(); /* Cast Packet */
	uint16_t from_id = from->u8[1] * 256 + from->u8[0];

	// test printf
	if (from_id == ID_LIGHT) {
		printf("Light Valueis from node = %d.%d are %u and %u\n", from->u8[0], from->u8[1], p->value1, p->value2);
		printf("temperature = %u\n", p->value3);
	}
	else if (from_id == ID_MOIST) {
		printf("moisture value from node %d.%d is %u\n", from->u8[0], from->u8[1], p->value1);
	}
        else if ( from_id == ID_CO2) {
                printf("CO2 value (adc) from node %d.%d is %u\n", from->u8[0], from->u8[1], p->value1);
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

	if (my_id == ID_MOIST)
  		SENSORS_ACTIVATE(vh400);
	else if (my_id == ID_LIGHT) {
		SENSORS_ACTIVATE(light_sensor);
		SENSORS_ACTIVATE(sht11_sensor);
	}
   else if (my_id == ID_CO2)
		SENSORS_ACTIVATE(ds1000_sensor);

	unicast_open(&uc, 140, &unicast_callbacks);

	if (my_id == ID_SINK) {
		// start actuators
		printf("PG:START\n");

		// add something smart here
		while(1) {
			PROCESS_YIELD();
 		}

		// stop actuators
		printf("PG:ENG\n");
	}

   while(1) {
		etimer_set(&et, CLOCK_SECOND * 5);
		PROCESS_WAIT_UNTIL(etimer_expired(&et));

		if (my_id == ID_MOIST) {
			p.value1 = vh400.value(ADC0);
			printf("my_id=%u moisture=%u\n", my_id, p.value1);
		}
		else if (my_id == ID_LIGHT) {
			p.value1 = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
			p.value2 = light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
			p.value3 = sht11_sensor.value(SHT11_SENSOR_TEMP);
			printf("my_id=%u total_solar=%u photosynthetic=%u\n" \
					"temperature = %u humidity = %u\n",my_id, p.value1, p.value2, p.value3, sht11_sensor.value(SHT11_SENSOR_TEMP));
		}
		else if (my_id == ID_CO2) {
			p.value1 = ds1000_sensor.value(SENSOR_CO2);
			printf("my_id=%u CO2 Value (adc) =%u\n" ,my_id, p.value1);
		}

		packetbuf_copyfrom(&p,sizeof(struct my_packet));

		unicast_send(&uc, &addr);	
    }
    PROCESS_END();
}
