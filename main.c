#include <stdio.h>
#include "contiki.h"
#include "rimeaddr.h"
#include "net/rime.h"
#include "vh400.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"
#include "ds1000-sensor.h"
#include "settings.h"

/* unicast packet ptructure */ 
struct my_packet{
	uint8_t type;
};

enum {
  COMMAND_TYPE_TEMP_LOW,
  COMMAND_TYPE_TEMP_OK,
  COMMAND_TYPE_CO2_HIGH,
  COMMAND_TYPE_LIGHT_LOW,
  COMMAND_TYPE_LIGHT_OK
};

#define MAX_RETRANSMISSIONS 4

static double raw_voltage=0;
static uint32_t ppm=0;
static uint32_t adc=0;
void print_values();

/* Unicast Receive Function */
static void
recv_runicast(struct runicast_conn *c, const rimeaddr_t *from, uint8_t seqno)
{
  struct my_packet * p;
  p=(struct my_packet *)packetbuf_dataptr();
  
  if(p->type == COMMAND_TYPE_TEMP_LOW) {
    printf("CO2 OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); }
  if(p->type == COMMAND_TYPE_CO2_HIGH) {
    printf("CO2 HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]); }
  if(p->type == COMMAND_TYPE_LIGHT_LOW) {
    printf("LIGHT LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); }

  
}
static void
sent_runicast(struct runicast_conn *c, const rimeaddr_t *to, uint8_t retransmissions)
{
  printf("runicast message sent to %d.%d, retransmissions %d\n",
	 to->u8[0], to->u8[1], retransmissions);
}
static void
timedout_runicast(struct runicast_conn *c, const rimeaddr_t *to, uint8_t retransmissions)
{
  printf("runicast message timed out when sending to %d.%d, retransmissions %d\n",
	 to->u8[0], to->u8[1], retransmissions);
}
/* runicast Connection */
static const struct runicast_callbacks runicast_callbacks = {recv_runicast,
							     sent_runicast,
							     timedout_runicast};
static struct runicast_conn uc;
/*---------------------------------------------------------------------------*/


PROCESS(main_process, "main");
AUTOSTART_PROCESSES(&main_process);

static struct etimer et;
static uint16_t my_id;
PROCESS_THREAD(main_process, ev, data)
{
	static struct my_packet p;
	static rimeaddr_t addr;
	

	PROCESS_EXITHANDLER(runicast_close(&uc);)
	PROCESS_BEGIN();

	my_id = rimeaddr_node_addr.u8[1] * 256 + rimeaddr_node_addr.u8[0];
	addr.u8[0] = ID_SINK % 256;
	addr.u8[1] = ID_SINK / 256;

	if (my_id == ID_MOIST1)
  		SENSORS_ACTIVATE(vh400);
	else if (my_id == ID_LIGHT1)
		SENSORS_ACTIVATE(light_sensor);
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

		if (my_id == ID_MOIST1) {
			print_values();
		}
		else if (my_id == ID_LIGHT1) {
			print_values()
		}
		else if (my_id == ID_CO2) {
			print_values()
		}

		packetbuf_copyfrom(&p,sizeof(struct my_packet));

		runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);	
    }
    PROCESS_END();
}

void print_values(){
  adc = ds1000_sensor.value(SENSOR_CO2);
  raw_voltage = (double)(adc/4096.0)*2.5;
  ppm = (raw_voltage * 1000) - 200;
	printf("CO2_reading: %u \n", ds1000_sensor.value(SENSOR_CO2));
	printf("Raw_Voltage = %d mV\n", (int)(1000*raw_voltage)); 
        printf("CO2 = %u ppm\n",ppm); 

}
