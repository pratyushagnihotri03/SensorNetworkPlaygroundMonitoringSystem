#include "co2.h"

void measure_co2(uint8_t state_co2[2])
{
	static uint16_t raw;
	static double co2;
	static uint8_t i;
	static state_co2[2];
	raw = ds1000_sensor.value(SENSOR_CO2);
	co2 = ((double)(raw/4096.0)*2.5 * 1000) - 200;
//	printf("CO2_ADC: ", adc);
//	printf("CO2_Raw_Voltage :", raw_voltage);  
	printf ("CO2 ppm: %d\n", (int)co2);

//------------------------------cO2 Actuators------------------------ //
	for (i = 0; i < 2; i++) {
		state_co2[i] = 0;

		if(state_co2[i] != COMMAND_TYPE_CO2_LOW && co2 < THRESHOLD_CO2_LOW[i] ){
			state_co2[i] = COMMAND_TYPE_CO2_LOW;
			continue;
		}
		else if (state_co2[i] != COMMAND_TYPE_CO2_HIGH && co2 > THRESHOLD_CO2_HIGH[i] ){
			state_co2[i] = COMMAND_TYPE_CO2_HIGH;
			continue;
		}
		else if (state_co2[i] != COMMAND_TYPE_CO2_OK 
			&& co2 <=THRESHOLD_CO2_HIGH[i] - OFFSET_CO2 
			&& co2 >= THRESHOLD_CO2_LOW[i] + OFFSET_CO2){
			state_co2[i] = COMMAND_TYPE_CO2_OK;
			continue;
		}
	}

	return;
}
