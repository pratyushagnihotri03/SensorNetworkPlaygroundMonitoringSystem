#include <stdio.h>
#include "contiki.h"
#include "net/rime.h"
#include "rimeaddr.h"
#include "ds1000-sensor.h"
#include "dev/leds.h"

/* Define a packet ptructure */ 
struct my_packet{
	uint8_t type;
};

enum {
  COMMAND_TYPE_CO2_OK,
  COMMAND_TYPE_CO2_HIGH,
  COMMAND_TYPE_LIGHT_LOW
};

static double raw_voltage=0;
static uint32_t ppm=0;
static uint32_t adc=0;
void print_values();

/*---------------------------------------------------------------------------*/
/* Unicast Receive Function */
static void
recv(struct unicast_conn *c, const rimeaddr_t *from)
{
  struct my_packet * p;
  p=(struct my_packet *)packetbuf_dataptr();
  
  if(p->type == COMMAND_TYPE_CO2_OK) {
    printf("CO2 OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); }
  if(p->type == COMMAND_TYPE_CO2_HIGH) {
    printf("CO2 HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]); }
  if(p->type == COMMAND_TYPE_LIGHT_LOW) {
    printf("LIGHT LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); }

  
}
/* Unicast Connection */
static const struct unicast_callbacks unicast_callbacks = {recv};
static struct unicast_conn uc;
/*---------------------------------------------------------------------------*/


PROCESS(test, "ds1000_test");
AUTOSTART_PROCESSES(&test);

static struct etimer et;

PROCESS_THREAD(test, ev, data)
{
	static int i;	
	static struct my_packet p;
        static rimeaddr_t addr;
	addr.u8[0] = 33;
        addr.u8[1] = 44;

        PROCESS_BEGIN();
	SENSORS_ACTIVATE(ds1000_sensor);
	unicast_open(&uc, 140, &unicast_callbacks);

	while(1) {
	etimer_set(&et, CLOCK_SECOND * 10);
    	PROCESS_WAIT_UNTIL(etimer_expired(&et));

    if (rimeaddr_node_addr.u8[0]== 210 && rimeaddr_node_addr.u8[1]== 117) {
        printf("I'm CO2 Sensor!\n");
        if (i) leds_on(LEDS_ALL); else leds_off(LEDS_ALL); i=(i?0:1);
	print_values();
        if (adc <=1300) {
	p.type = COMMAND_TYPE_CO2_OK;
        packetbuf_copyfrom(&p,sizeof(struct my_packet));
        printf("Sending CO2 OK to Sink...\n");
        unicast_send(&uc, &addr);	

}

 if (adc >1300) {
	p.type = COMMAND_TYPE_CO2_HIGH;
        packetbuf_copyfrom(&p,sizeof(struct my_packet));
        printf("Sending CO2 HIGH to Sink...\n");
        unicast_send(&uc, &addr);	

}

} 
	else printf("Sink is waiting for Data...\n");

}
        
        PROCESS_END();
}

void print_values(){
  adc = ds1000_sensor.value(SENSOR_CO2);
  raw_voltage = (double)(adc/4096.0)*2.5;
  ppm = (raw_voltage * 1000) - 200;
	printf("CO2_reading: %u \n", ds1000_sensor.value(SENSOR_CO2));
	printf("Raw_Voltage = %d V\n", (int)(1000*raw_voltage)); 
        printf("CO2 = %u ppm\n",ppm); 

}
