 /**
 *  \file
 *       Test program and usage example of the DS1000 sensorboard contiki driver.
 *
 *  \author	 
 *       Eloy Díaz <eldial@gmail.com>
 */
#include "contiki.h"
#include "rimeaddr.h"

#include "ds1000-sensor.h"
#include "dev/leds.h"

#include <stdio.h> 

void print_values();

/*---------------------------------------------------------------------------*/
PROCESS(ds1000_test, "DS1000 Test");
AUTOSTART_PROCESSES(&ds1000_test);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ds1000_test, ev, data)
{
  PROCESS_BEGIN();

  static struct etimer et;
	
  printf ("Activating Sensor...\n");
  SENSORS_ACTIVATE(ds1000_sensor);
  
  static int i;

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 2);
    PROCESS_WAIT_UNTIL(etimer_expired(&et));
    if (i) leds_on(LEDS_ALL); else leds_off(LEDS_ALL); i=(i?0:1);
    print_values();
  }

  PROCESS_END();
}

void print_values(){
  // print moteid, CO2, TEMP, CO
  uint16_t complete_id = rimeaddr_node_addr.u8[1] * 256 + rimeaddr_node_addr.u8[0];
	printf("%4u CO2: %10u TMP: %10u CO:%10u\n", complete_id, ds1000_sensor.value(SENSOR_CO2),ds1000_sensor.value(SENSOR_TEMP),ds1000_sensor.value(SENSOR_CO)); 

}
/*---------------------------------------------------------------------------*/
