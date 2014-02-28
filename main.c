#include <stdio.h>
#include "contiki.h"
#include "rimeaddr.h"
#include "net/rime.h"
#include "dev/sky-sensors.h"
#include "sink.h"
#include "moisture.h"
#include "light.h"
#include "co2.h"

#define MAX_RETRANSMISSIONS 4

/* Unicast Receive Function */
static void
recv_runicast(struct runicast_conn *c, const rimeaddr_t *from, uint8_t seqno)
{
  struct my_packet * p;
  p=(struct my_packet *)packetbuf_dataptr();
//------------------ Temperature--------------------
  if(p->type == COMMAND_TYPE_TEMP_LOW) {
    printf("TEMP LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]);
	//Turning on the HEATER.
   	 printf("PG:HEAT ON\n");  
	 }
  else if(p->type == COMMAND_TYPE_TEMP_OK) {
    printf("TEMP OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//Turning off the Fan and Heater.
   	 printf("PG:FAN OFF\n");
	 printf("PG:HEAT OFF\n");  
	}
  else if(p->type == COMMAND_TYPE_TEMP_HIGH) {
    printf("TEMP HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]);
	 //Turning on the Fan.
   	 printf("PG:FAN ON\n");  
	}
//------------------- CO2---------------------------
  
  if(p->type == COMMAND_TYPE_CO2_HIGH) {
    printf("CO2 HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
    	//Turning on the Fan.
   	 printf("PG:FAN ON\n");  
	}
  else if(p->type == COMMAND_TYPE_CO2_OK) {
    printf("CO2 OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//Turning off the Fan.
   	 printf("PG:FAN OFF\n"); 
	}
  else if(p->type == COMMAND_TYPE_CO2_LOW) {
    printf("CO2 LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//GIVE CO2.
   	 printf(" ! GIVE CO2 !\n"); 
	}
//------------------- LIGHT--------------------------- 
  if(p->type == COMMAND_TYPE_LIGHT_LOW) {
    printf("LIGHT LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
 	//Turning on the lamp.
   	 printf("PG:LIGHT ON\n");
	}
  else if(p->type == COMMAND_TYPE_LIGHT_OK) {
    printf("LIGHT OK! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning off the lamp.
   	 printf("PG:LIGHT OFF\n");
	}

//-------------------HUMIDITY---------------------------

  if(p->type == COMMAND_TYPE_HUMID_LOW) {
    printf("HUMIDITY LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
 	//Turning on the Heater.
   	 printf("PG:HEAT ON\n");
	}
  else if(p->type == COMMAND_TYPE_HUMID_OK) {
    printf("HUMIDITY OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//Turning off the Fan and Heater.
   	 printf("PG:FAN OFF\n");
	 printf("PG:HEAT OFF\n");  
	}
  else if(p->type == COMMAND_TYPE_HUMID_HIGH) {
    printf("HUMIDITY HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning on the Fan.
   	 printf("PG:FAN ON\n"); 
       }

//-------------------SOIL MOISTURE-----------------------

  if(p->type == COMMAND_TYPE_MOIS_LOW) {
    printf("MOSITURE LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
 	//Give water to the plant.
   	 printf("! GIVE WATER !\n");
	}
  else if(p->type == COMMAND_TYPE_MOIS_OK) {
    printf("MOSITURE OK! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning off the heater.
   	 printf("PG:HEAT OFF\n"); 
	}
  else if(p->type == COMMAND_TYPE_MOIS_HIGH) {
    printf("MOSITURE HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning on the heater.
   	 printf("PG:HEAT ON\n");
        }
  
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
	static uint8_t cmd = 0;

	PROCESS_EXITHANDLER(runicast_close(&uc);)
	PROCESS_BEGIN();

	my_id = rimeaddr_node_addr.u8[1] * 256 + rimeaddr_node_addr.u8[0];
	addr.u8[0] = ID_SINK % 256;
	addr.u8[1] = ID_SINK / 256;

	//wait for raspberry pi
	etimer_set(&et, CLOCK_SECOND * 120);
	PROCESS_WAIT_UNTIL(etimer_expired(&et));

	if (my_id == ID_MOIST)
		SENSORS_ACTIVATE(vh400);
	else if (my_id == ID_LIGHT) {
		SENSORS_ACTIVATE(light_sensor);
		SENSORS_ACTIVATE(sht11_sensor);
	}
	else if (my_id == ID_CO2)
	SENSORS_ACTIVATE(ds1000_sensor);

	runicast_open(&uc, 140, &runicast_callbacks);

	if (my_id == ID_SINK) {
		sink();
	}

	while(1) {
		etimer_set(&et, CLOCK_SECOND * MEASURING_PERIOD);
		PROCESS_WAIT_UNTIL(etimer_expired(&et));

		if (my_id == ID_MOIST)
			cmd = measure_moisture();
		else if (my_id == ID_LIGHT) {  //internal sensors node
			cmd = measure_light(); 
			if (cmd != 0) {
				p.type = cmd;
				packetbuf_copyfrom(&p,sizeof(struct my_packet));
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}

			cmd = measure_temperature();
			if (cmd != 0) {
				p.type = cmd;
				packetbuf_copyfrom(&p,sizeof(struct my_packet));
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}

			cmd = measure_humidity();
			if (cmd != 0) {
				p.type = cmd;
				packetbuf_copyfrom(&p,sizeof(struct my_packet));
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
		}
		else if (my_id == ID_CO2)
			cmd = measure_co2();

		if (cmd != 0) {
			p.type = cmd;
			packetbuf_copyfrom(&p,sizeof(struct my_packet));
			runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
		}
	}
	PROCESS_END();
}
