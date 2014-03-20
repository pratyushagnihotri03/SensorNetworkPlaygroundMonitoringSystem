#include "co2.h"

void measure_co2(uint8_t cmd[2])
{
	static uint32_t raw;
	static double co2;
	static uint8_t i;
	static uint8_t state_co2[2] = {CO2_OK,CO2_OK};
	
	raw = 0;
	for (i=0; i < 3; i++)
		raw += ds1000_sensor.value(SENSOR_CO2);
	raw /= 3;

	co2 = ((double)(raw/4096.0)*2.5 * 1000) - 200;
//	printf("CO2_ADC: ", adc);
//	printf("CO2_Raw_Voltage :", raw_voltage);  
	printf ("CO2 ppm: %d\n", (int)co2);

//------------------------------cO2 Actuators------------------------ //
	for (i = 0; i < 2; i++) {
		cmd[i] = 0;

		if(state_co2[i] != CO2_LOW && co2 < THRESHOLD_CO2_LOW[i] ){
			if (state_co2[i] == CO2_HIGH)
				cmd[i] = CO2_OK;
			state_co2[i] = CO2_LOW;
			printf("%s C02 low\n", plant_name[i]);
			continue;
		}
		else if (state_co2[i] != CO2_HIGH && co2 > THRESHOLD_CO2_HIGH[i] ){
			state_co2[i] = CO2_HIGH;
			cmd[i] = CO2_HIGH;
			continue;
		}
		else if (co2 <=THRESHOLD_CO2_HIGH[i] - OFFSET_CO2 
			&& co2 >= THRESHOLD_CO2_LOW[i] + OFFSET_CO2){
			if (state_co2[i] == CO2_LOW) {
				state_co2[i] = CO2_OK;
				printf("%s C02 ok\n", plant_name[i]);
			} else if (state_co2[i] == CO2_HIGH) {
				state_co2[i] = CO2_OK;
				cmd[i] = CO2_OK;
			}
			continue;
		}
	}

	return;
}
