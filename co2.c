#include "co2.h"

uint8_t measure_co2()
{
	uint8_t raw;
	double co2;

	raw = ds1000_sensor.value(SENSOR_CO2);
	co2 = ((double)(raw/4096.0)*2.5 * 1000) - 200;
//	printf("CO2_ADC: ", adc);
//	printf("CO2_Raw_Voltage :", raw_voltage);  
	printf ("CO2 ppm: %d", (int)co2);
//------------------------------cO2 Actuators------------------------ //
	if(co2 < THRESHOLD_CO2_LOW ){
		return COMMAND_TYPE_CO2_LOW;
	}
	else if (co2 > THRESHOLD_CO2_HIGH ){
		return COMMAND_TYPE_CO2_HIGH;
	}
	else if (co2 <=THRESHOLD_CO2_HIGH   && co2 >= THRESHOLD_CO2_LOW){
		return COMMAND_TYPE_CO2_OK;
	}

	return 0;
}
