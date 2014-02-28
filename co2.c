#include "co2.h"

uint8_t measure_co2()
{
	static uint8_t raw;
	static double co2;
	static uint8_t state_co2 = 0;

	raw = ds1000_sensor.value(SENSOR_CO2);
	co2 = ((double)(raw/4096.0)*2.5 * 1000) - 200;
//	printf("CO2_ADC: ", adc);
//	printf("CO2_Raw_Voltage :", raw_voltage);  
	printf ("CO2 ppm: %d\n", (int)co2);
//------------------------------cO2 Actuators------------------------ //
	if(state_co2 != COMMAND_TYPE_CO2_LOW && co2 < THRESHOLD_CO2_LOW ){
		return state_co2 = COMMAND_TYPE_CO2_LOW;
	}
	else if (state_co2 != COMMAND_TYPE_CO2_HIGH && co2 > THRESHOLD_CO2_HIGH ){
		return state_co2 = COMMAND_TYPE_CO2_HIGH;
	}
	else if (state_co2 != COMMAND_TYPE_CO2_OK 
		&& co2 <=THRESHOLD_CO2_HIGH - OFFSET_CO2 
		&& co2 >= THRESHOLD_CO2_LOW + OFFSET_CO2){
		return state_co2 = COMMAND_TYPE_CO2_OK;
	}

	return 0;
}
