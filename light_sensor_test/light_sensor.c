#include <stdio.h>
#include "contiki.h"
#include "net/rime.h"
#include "rimeaddr.h"
#include "dev/leds.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"

/* Define a packet ptructure */ 
struct my_packet{
	uint16_t  light_value_PHOTOSYNTHETIC;
	uint16_t  light_value_TOTALSOLAR;
};
/*---------------------------------------------------------------------------*/
/* Unicast Receive Function */
static void
recv(struct unicast_conn *c, const rimeaddr_t *from)
{
  struct my_packet * p;
 
  p=(struct my_packet *)packetbuf_dataptr(); /* Cast Packet */ 
  printf("Light Value Received from node = %d.%d \n", from->u8[0], from->u8[1]);
  printf("light_value_TOTALSOLAR =    %u\n",p->light_value_TOTALSOLAR); 
  
  
}
/* Unicast Connection */
static const struct unicast_callbacks unicast_callbacks = {recv};
static struct unicast_conn uc;
/*---------------------------------------------------------------------------*/


PROCESS(test, "light_test");
AUTOSTART_PROCESSES(&test);

static struct etimer et;

PROCESS_THREAD(test, ev, data)
{
	static struct my_packet p;
        rimeaddr_t addr;

        PROCESS_BEGIN();
	SENSORS_ACTIVATE(light_sensor);
	unicast_open(&uc, 140, &unicast_callbacks);
	printf("Starting Test:\n");
	
        while(1) {
	etimer_set(&et, CLOCK_SECOND * 5);
    	PROCESS_WAIT_UNTIL(etimer_expired(&et));

	p.light_value_TOTALSOLAR = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
  	printf("light_value_TOTALSOLAR my sensor = %u\n",p.light_value_TOTALSOLAR); 
  


    packetbuf_copyfrom(&p,sizeof(struct my_packet));
    addr.u8[0] = 201; // TUdunet Xm1000 node id = 201
    addr.u8[1] = 0;
    /* All nodes send to sink except sink itself */
      printf("MY ADDress  %u\n, %u\n",rimeaddr_node_addr.u8[0],rimeaddr_node_addr.u8[1]);
    if(!rimeaddr_cmp(&addr, &rimeaddr_node_addr)) {
      printf("Sending to Sink\n");
      unicast_send(&uc, &addr);
    }

}
        
        PROCESS_END();
}
