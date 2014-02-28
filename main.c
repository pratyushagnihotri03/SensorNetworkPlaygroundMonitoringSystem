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
	uint8_t type;
};

enum {
//TEMP
  COMMAND_TYPE_TEMP_LOW,
  COMMAND_TYPE_TEMP_OK,
  COMMAND_TYPE_TEMP_HIGH,
//CO2
  COMMAND_TYPE_CO2_LOW,
  COMMAND_TYPE_CO2_OK,
  COMMAND_TYPE_CO2_HIGH,
//LIGHT
  COMMAND_TYPE_LIGHT_LOW,
  COMMAND_TYPE_LIGHT_OK,
//HUMID
  COMMAND_TYPE_HUMID_LOW,
  COMMAND_TYPE_HUMID_OK,
  COMMAND_TYPE_HUMID_HIGH,
//MOSITURE
  COMMAND_TYPE_MOIS_LOW,
  COMMAND_TYPE_MOIS_OK,
  COMMAND_TYPE_MOIS_HIGH
};

#define MAX_RETRANSMISSIONS 4

void print_values();

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
	static uint16_t value1, value2, value3;
	static int temp_val=0,humi_val=0;
	static double raw_voltage = 0, moisture;
	static uint32_t adc=0;

	PROCESS_EXITHANDLER(runicast_close(&uc);)
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

	runicast_open(&uc, 140, &runicast_callbacks);

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
			int v = vh400.value(ADC0);
			raw_voltage = 3 * v / 4096.0;

			if (raw_voltage < 1.1)
				moisture = 10 * raw_voltage - 1;
			else if (raw_voltage < 1.3)
				moisture = 25 * raw_voltage - 17.5;
			else if (raw_voltage < 1.82)
				moisture = 48.08 * raw_voltage - 47.5;
			else
				moisture = 26.32 * raw_voltage - 7.89;

			printf ("Soil Moisture: %u.%u\n", (int)moisture, (int)(moisture * 100) % 100);

//------------------------------MOISTURE Actuators------------------------ //
			if(moisture < THRESHOLD_MOIS_LOW ){
				p.type = COMMAND_TYPE_MOIS_LOW;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (moisture > THRESHOLD_MOIS_HIGH ){
				p.type = COMMAND_TYPE_MOIS_HIGH;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (moisture <=THRESHOLD_MOIS_HIGH   && moisture >= THRESHOLD_MOIS_LOW){
				p.type = COMMAND_TYPE_MOIS_OK;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
		}
		else if (my_id == ID_LIGHT) {
			value1 = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
			value2 = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
			value3 = sht11_sensor.value(SHT11_SENSOR_TEMP);
			temp_val= ((0.01 * value3 -32)* .55);
			double humidity_val= (-4.0 + 405.0*value2/10000);
			humi_val = (int) humidity_val;
		    
			print_values("Light_Sensor_ADC :", value1);
			print_values("Humidity_ADC :", value2);
			print_values("Humidity_Value :", humi_val );
			print_values("Temperature_ADC :", value3);
			print_values("Temperature °C :", temp_val);

//------------------------------Light Actuators------------------------ //

			if(value1 < THRESHOLD_LIGHT ){
				p.type = COMMAND_TYPE_LIGHT_LOW;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else {
				p.type = COMMAND_TYPE_LIGHT_OK;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
//------------------------------Temperature Actuators------------------------ //
			if(value3 < THRESHOLD_TEMP_LOW ){
				p.type = COMMAND_TYPE_TEMP_LOW;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (value3 > THRESHOLD_TEMP_HIGH ){
				p.type = COMMAND_TYPE_TEMP_HIGH;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (value3 <=THRESHOLD_TEMP_HIGH   && value3 >= THRESHOLD_TEMP_LOW ){
				p.type = COMMAND_TYPE_TEMP_OK;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
//------------------------------HUMIDITY Actuators------------------------ //
			if(value2 < THRESHOLD_HUMID_LOW ){
				p.type = COMMAND_TYPE_HUMID_LOW;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (value2 > THRESHOLD_HUMID_HIGH ){
				p.type = COMMAND_TYPE_HUMID_HIGH;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (value2 <=THRESHOLD_HUMID_HIGH   && value2 >= THRESHOLD_HUMID_LOW){
				p.type = COMMAND_TYPE_HUMID_OK;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			
			
		}
		else if (my_id == ID_CO2) {

			adc = ds1000_sensor.value(SENSOR_CO2);
  			raw_voltage = (double)(adc/4096.0)*2.5; 
			value1 = ((double)(adc/4096.0)*2.5 * 1000) - 200;
			printf("CO2_ADC: ", adc);
			printf("CO2_Raw_Voltage :", raw_voltage);  
			print_values("CO2 ppm:", value1);
//------------------------------cO2 Actuators------------------------ //
			if(value1 < THRESHOLD_CO2_LOW ){
				p.type = COMMAND_TYPE_CO2_LOW;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (value1 > THRESHOLD_CO2_HIGH ){
				p.type = COMMAND_TYPE_CO2_HIGH;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
			else if (value1 <=THRESHOLD_CO2_HIGH   && value1 >= THRESHOLD_CO2_LOW){
				p.type = COMMAND_TYPE_CO2_OK;
				runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);
			}
				
		}

		packetbuf_copyfrom(&p,sizeof(struct my_packet));

		runicast_send(&uc, &addr, MAX_RETRANSMISSIONS);	
    }
    PROCESS_END();
}

void print_values(char * name, uint16_t value){
	printf("%s %u\n", name, value);
}
