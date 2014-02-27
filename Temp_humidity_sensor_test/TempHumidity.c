#include "net/rime.h"
#include "rimeaddr.h"
#include "contiki.h" 
#include <stdio.h>
#include <math.h>
#include "dev/sht11-sensor.h"

/* Define a packet ptructure */ 
struct sensor_data
{	
        uint32_t   temperature;	
        uint32_t   humidity;
};

/*---------------------------------------------------------------------------*/
	
/* Unicast Receive Function */	
static void recv(struct unicast_conn *c, const rimeaddr_t *from)
{	
	struct sensor_data * data_to_send;
	data_to_send = (struct sensor_data *)packetbuf_dataptr(); /* Cast Packet */ 
	printf("Temperature and humidity value received from node : %d.%d \n", from->u8[0], from->u8[1]);
	printf("Temperature value : %u\n",data_to_send->temperature); 
	printf("Humidity value : %u\n",data_to_send->humidity); 	
}
	
/* Unicast Connection */
	
static const struct unicast_callbacks unicast_callbacks = {recv};	
static struct unicast_conn uc;
	
/*---------------------------------------------------------------------------*/


PROCESS(sense_ts_process, "temperature-humidity readings"); 
AUTOSTART_PROCESSES(&sense_ts_process); 

static double get_temp(void)
{
	return ( (-39.8) + (sht11_sensor.value(SHT11_SENSOR_TEMP) * 0.01) );
}

static double get_humidity(void)
{	
	uint32_t sens_value;	
	sens_value     = sht11_sensor.value(SHT11_SENSOR_HUMIDITY); 	
	double hum_val = (-2.0468 +
			 (0.0367 * sens_value) + 
			 ( -1.5955E-6 * (pow(sens_value,2)) ) );
	return hum_val;
}

PROCESS_THREAD(sense_ts_process, ev, data) 
{  
	PROCESS_BEGIN();  
	static struct sensor_data p;
	rimeaddr_t addr;
	
	static struct etimer et;  
	//static float temp_val, humidity_val;
	//static uint32_t humidity, temp; 

	SENSORS_ACTIVATE(sht11_sensor); 
	clock_init(); 	
	while( 1) 
	{    
		etimer_set(& et, CLOCK_SECOND*5);    
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(& et));   
		
		//humidity	= sht11_sensor.value(SHT11_SENSOR_HUMIDITY); 
		//temp		= sht11_sensor.value(SHT11_SENSOR_TEMP);
		
		//temp_val	= get_temp(); 
		//humidity_val	= get_humidity();
			
		//printf("Temperature : %u.%u, raw : %u\n\r", (int)temp_val,((int)(temp_val*10))%10, temp);
		//printf("Humidity : %u.%u, raw : %u\n\r", (int)humidity_val,((int)(humidity_val*10))%10, humidity);
		printf("\n");

		p.temperature = get_temp(); 
		p.humidity    = get_humidity();
		printf("Temperature : %u\n\r", p.temperature);
		printf("Humidity : %u\n\r", p.humidity);

		packetbuf_copyfrom(&p,sizeof(struct sensor_data));	

		addr.u8[0] = 201; // TUdunet Xm1000 node id = 201
		addr.u8[1] = 0;
	
		/* All nodes send to sink except sink itself */

		if(!rimeaddr_cmp(&addr, &rimeaddr_node_addr)) 
		{
      			printf("Sending to Sink\n");
	        	unicast_send(&uc, &addr);
		}  
	}	
	SENSORS_DEACTIVATE(sht11_sensor); 
	PROCESS_END(); 

}
