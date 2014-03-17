#include <stdio.h>
#include "contiki.h"
#include "rimeaddr.h"
#include "net/rime.h"
#include "moisture.h"
#include "light.h"
#include "co2.h"

#define MAX_RETRANSMISSIONS 4

/* set threshold values for {RIGHT, LEFT} plant here */
const double THRESHOLD_CO2_HIGH[] = { 950, 950 };
const double THRESHOLD_CO2_LOW[] = { 40, 40 };
const uint32_t THRESHOLD_LIGHT[] = { 300, 400 };
const double THRESHOLD_TEMP_HIGH[] = { 28, 29 };
const double THRESHOLD_TEMP_LOW[] = { 24, 12.7 };
const uint16_t THRESHOLD_MOIS_LOW[] = { 1774, 1502 };
const uint16_t THRESHOLD_MOIS_HIGH[] = { 2428, 1917 };
const uint16_t THRESHOLD_HUMID_LOW[] = { 20, 20 };
const uint16_t THRESHOLD_HUMID_HIGH[] = { 40, 40 };
const char * plant_name[] = {"Peperomia", "Kalanchoe"};

/* Unicast Receive Function */
static void
recv_runicast(struct runicast_conn *c, const rimeaddr_t *from, uint8_t seqno)
{
printf("received something\n");
  struct my_packet * p;
  p=(struct my_packet *)packetbuf_dataptr();
//------------------ Temperature--------------------
  if(p->type == TEMP_LOW) {
    printf("TEMP LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]);
	//not Turning on the HEATER.
//fixme: don't send message, just print "temp low"
   	 printf("Temperature low\n");  
	 }
  else if(p->type == TEMP_OK) {
    printf("TEMP OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//not Turning off the Heater.
	 printf("Temperature ok\n");  
	}
  else if(p->type == TEMP_HIGH) {
    printf("TEMP HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]);
	 //Turning on the Fan.
  // 	 printf("PG:FAN ON\n");  
	}
//------------------- CO2---------------------------
  
  if(p->type == CO2_HIGH) {
    printf("CO2 HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
    	//Turning on the Fan.
   	 printf("PG:FAN ON\n");  
	}
  else if(p->type == CO2_OK) {
    printf("CO2 OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//Turning off the Fan.
   	 printf("PG:FAN OFF\n"); 
	}
  else if(p->type == CO2_LOW) {
    printf("CO2 LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//GIVE CO2.
   //	 printf(" ! GIVE CO2 !\n"); 
	}
//------------------- LIGHT--------------------------- 
  if(p->type == LIGHT_LOW) {
    printf("LIGHT LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
 	//Turning on the lamp.
   	 printf("PG:LIGHT ON\n");
	}
  else if(p->type == LIGHT_OK) {
    printf("LIGHT OK! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning off the lamp.
   	 printf("PG:LIGHT OFF\n");
	}

//-------------------HUMIDITY---------------------------

  if(p->type == HUMID_LOW) {
    printf("HUMIDITY LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
 	//Turning on the Heater.
   //	 printf("PG:HEAT ON\n");
	}
  else if(p->type == HUMID_OK) {
    printf("HUMIDITY OK! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
	//Turning off the Fan.
   	 printf("PG:FAN OFF\n");
	}
  else if(p->type == HUMID_HIGH) {
    printf("HUMIDITY HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning on the Fan.
   	 printf("PG:FAN ON\n"); 
       }

//-------------------SOIL MOISTURE-----------------------

  if(p->type == MOIS_LOW) {
    printf("MOSITURE LOW! received from %d.%d\n",
           from->u8[0], from->u8[1]); 
 	//Give water to the plant.
   	 //printf("! GIVE WATER !\n");
	}
  else if(p->type == MOIS_OK) {
    printf("MOSITURE OK! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning off the heater.
   	 printf("PG:HEAT OFF\n"); 
	}
  else if(p->type == MOIS_HIGH) {
    printf("MOSITURE HIGH! received from %d.%d\n",
           from->u8[0], from->u8[1]);
 	//Turning on the heater.
   	 printf("PG:HEAT ON\n");
        }
  
}
static void
sent_runicast(struct runicast_conn *c, const rimeaddr_t *to, uint8_t retransmissions)
{
//  printf("runicast message sent to %d.%d, retransmissions %d\n",
//	 to->u8[0], to->u8[1], retransmissions);
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
	static rimeaddr_t addr[2];
	static uint8_t cmd[2] = {0,0};
	static int offset = 0, i;

	PROCESS_EXITHANDLER(runicast_close(&uc);)
	PROCESS_BEGIN();
	runicast_open(&uc, 140, &runicast_callbacks);

	my_id = rimeaddr_node_addr.u8[1] * 256 + rimeaddr_node_addr.u8[0];
	addr[RIGHT].u8[0] = ID_SINK_R % 256;
	addr[RIGHT].u8[1] = ID_SINK_R / 256;
	addr[LEFT].u8[0] = ID_SINK_L % 256;
	addr[LEFT].u8[1] = ID_SINK_L / 256;

	if (my_id == ID_MOIST_R || my_id == ID_MOIST_L) {
		offset = 1;
		SENSORS_ACTIVATE(vh400);
	}
	else if (my_id == ID_LIGHT) {
		offset = 2;
		SENSORS_ACTIVATE(light_sensor);
		SENSORS_ACTIVATE(sht11_sensor);
	}
	else if (my_id == ID_CO2) {
		offset = 3;
		SENSORS_ACTIVATE(ds1000_sensor);
	}


	//wait for raspberry pi
	etimer_set(&et, CLOCK_SECOND * (120 + offset));
	PROCESS_WAIT_UNTIL(etimer_expired(&et));


	if (my_id == ID_SINK_R || my_id == ID_SINK_L) {
		// start actuators
		printf("PG:START\n");

		// smart things are done in recv callback function
		while(1) {
			PROCESS_YIELD();
		}

		// never stop actuators??!?
		printf("PG:ENG\n");
	}

	while(1) {
		etimer_set(&et, CLOCK_SECOND * MEASURING_PERIOD);
		PROCESS_WAIT_UNTIL(etimer_expired(&et));

		if (my_id == ID_MOIST_R) {
			measure_moisture(my_id);
			if (cmd[RIGHT] != 0) {
				p.type = cmd[RIGHT];
				packetbuf_copyfrom(&p,sizeof(struct my_packet));
				runicast_send(&uc, &addr[RIGHT], MAX_RETRANSMISSIONS);
			}
		}
		if (my_id == ID_MOIST_L) {
			measure_moisture(my_id);
			if (cmd[LEFT] != 0) {
				p.type = cmd[LEFT];
				packetbuf_copyfrom(&p,sizeof(struct my_packet));
				runicast_send(&uc, &addr[LEFT], MAX_RETRANSMISSIONS);
			}
		}
		else if (my_id == ID_LIGHT) {  //internal sensors node
			measure_light(cmd); 
			for (i = 0; i < 2; i++) {
				if (cmd[i] != 0) {
					p.type = cmd[i];
					packetbuf_copyfrom(&p,sizeof(struct my_packet));
					runicast_send(&uc, &addr[i], MAX_RETRANSMISSIONS);
//fixme 20ms delay
					printf("sent light cmd %u to sink #%d, addr=%u.%u\n", cmd[i], i, addr[i].u8[0], addr[i].u8[1]);
				}
			}

			measure_temperature(cmd);
			for (i = 0; i < 2; i++) {
				if (cmd[i] != 0) {
					p.type = cmd[i];
					packetbuf_copyfrom(&p,sizeof(struct my_packet));
					runicast_send(&uc, &addr[i], MAX_RETRANSMISSIONS);
				}
			}

			measure_humidity(cmd);
			for (i = 0; i < 2; i++) {
				if (cmd[i] != 0) {
					p.type = cmd[i];
					packetbuf_copyfrom(&p,sizeof(struct my_packet));
					runicast_send(&uc, &addr[i], MAX_RETRANSMISSIONS);
				}
			}
		}
		else if (my_id == ID_CO2) {
			measure_co2(cmd);
			for (i = 0; i < 2; i++) {
				if (cmd[i] != 0) {
					p.type = cmd[i];
					packetbuf_copyfrom(&p,sizeof(struct my_packet));
					runicast_send(&uc, &addr[i], MAX_RETRANSMISSIONS);
					printf("sent co2 cmd to sink #%d, addr=%u.%u\n", i, addr[i].u8[0], addr[i].u8[1]);
				}
			}
		}
		
	}
	PROCESS_END();
}
