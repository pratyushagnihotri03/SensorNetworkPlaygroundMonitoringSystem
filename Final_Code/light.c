#include "light.h"

/* global temperature value for  *
 * relative humidity calculation */
static double temperature = 20.0;

void measure_light(uint8_t cmd[2])
{
	static uint32_t raw_light;
	static uint8_t i;
	static uint16_t light;
	static uint8_t state_light[2] = {LIGHT_OK, LIGHT_OK};

	raw_light = 0;
	for (i=0; i < 3; i++)
		raw_light += (uint32_t)light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR); // changed as requested by iliya. better:PHOTOSYNTHETIC
	raw_light /= 3;

	light = (uint16_t)(((3125 * raw_light) >> 9) & 0xFFFF);
	printf("Light/lux: %u\n", light);


//------------------------------Light Actuators------------------------ //

	for (i = 0; i < 2; i++) {
		cmd[i] = 0;
		if(state_light[i] != LIGHT_LOW && raw_light < THRESHOLD_LIGHT[i]){
			state_light[i] = LIGHT_LOW;
			cmd[i] = LIGHT_LOW;
			continue;
		}
		else if (state_light[i] != LIGHT_OK
			&& raw_light >= THRESHOLD_LIGHT[i] + OFFSET_LIGHT) {
			state_light[i] = LIGHT_OK;
			cmd[i] = LIGHT_OK;
			continue;
		}
	}

	return;
}

void measure_humidity()
{
	static uint32_t raw_humidity;
	static double humidity_val;
	static uint8_t i;
   	static uint8_t state_humidity[2] = {HUMID_OK, HUMID_OK};

	raw_humidity = 0;
	for (i=0; i < 3; i++)
		raw_humidity += sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
	raw_humidity /= 3;

	humidity_val = (temperature - 25) * (0.01 + 0.00008 * raw_humidity)
						- 2.0468 + 0.0367 * raw_humidity - 1.5955E-6 * pow(raw_humidity, 2);

	//limit humidty to 100%, according to SHT11 data sheet
	if (humidity_val > 100)
		humidity_val = 100;

	//print_values("Humidity_ADC :", value2);
	printf("Humidity: %u.%u\n", (int)humidity_val, (int)(100* humidity_val)%100);

//------------------------------HUMIDITY Actuators------------------------ //

	for (i = 0; i < 2; i++) {
		if(state_humidity[i] != HUMID_LOW 
			&& raw_humidity < THRESHOLD_HUMID_LOW[i]) {
			state_humidity[i] = HUMID_LOW;
			printf("%s Humidity low\n", plant_name[i]);
			continue;
		}
		else if (state_humidity[i] != HUMID_HIGH
			&& raw_humidity > THRESHOLD_HUMID_HIGH[i]) {
			state_humidity[i] = HUMID_HIGH;
			printf("%s Humidity high\n", plant_name[i]);
			continue;
		}
		else if (state_humidity[i] != HUMID_OK
			&& raw_humidity <= THRESHOLD_HUMID_HIGH[i] - OFFSET_HUMIDITY 
			&& raw_humidity >= THRESHOLD_HUMID_LOW[i] + OFFSET_HUMIDITY){
			state_humidity[i] = HUMID_OK;
			printf("%s Humidity ok\n", plant_name[i]);
			continue;
		}
	}

	return;
}

void measure_temperature()
{
	static uint32_t raw_temp;
	static uint8_t i;
	static int temp2;
	static uint8_t state_temp[2] = {TEMP_OK, TEMP_OK};

	raw_temp = 0;
	for (i=0; i < 3; i++)
		raw_temp += sht11_sensor.value(SHT11_SENSOR_TEMP);
	raw_temp /= 3;

	temperature = 0.01 * raw_temp - 39.6;

	temp2 = (int)(temperature * -100) % 100;
	if (temp2 < 0)
		temp2 *= -1;

	printf("Temperature °C: %d.%d\n", (int)temperature, temp2);

//------------------------------Temperature Actuators------------------------ //

	for (i = 0; i < 2; i++) {
		if(state_temp[i] != TEMP_LOW
			&& temperature < THRESHOLD_TEMP_LOW[i] ){
			state_temp[i] = TEMP_LOW;
			printf("%s Temperature low\n", plant_name[i]);
			continue;
		}
		else if (state_temp[i] != TEMP_HIGH
			&& temperature > THRESHOLD_TEMP_HIGH[i] ){
			state_temp[i] = TEMP_HIGH;
			printf("%s Temperature high\n", plant_name[i]);
			continue;
		}
		else if (state_temp[i] != TEMP_OK
			&& temperature <= THRESHOLD_TEMP_HIGH[i] - OFFSET_TEMP
			&& temperature >= THRESHOLD_TEMP_LOW[i] + OFFSET_TEMP) {
			state_temp[i] = TEMP_OK;
			printf("%s Temperature ok\n", plant_name[i]);
			continue;
		}
	}

	return;
}
