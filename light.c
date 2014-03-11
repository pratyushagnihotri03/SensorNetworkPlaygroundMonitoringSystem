#include "light.h"

/* global temperature value for  *
 * relative humidity calculation */
static double temperature;

uint8_t measure_light()
{
	static uint32_t raw_light;
	static uint8_t state_light = 0;

	raw_light = (uint32_t)light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
	printf("raw %u Light/lux: %u\n", (uint16_t)raw_light, (uint16_t)(((3125 * raw_light) >> 9) & 0xFFFF));


//------------------------------Light Actuators------------------------ //

	if(state_light != COMMAND_TYPE_LIGHT_LOW && raw_light < THRESHOLD_LIGHT){
		return state_light = COMMAND_TYPE_LIGHT_LOW;
	}
	else if (state_light != COMMAND_TYPE_LIGHT_OK
		&& raw_light >= THRESHOLD_LIGHT + OFFSET_LIGHT) {
		return state_light = COMMAND_TYPE_LIGHT_OK;
	}

	return 0;
}

uint8_t measure_humidity()
{
	static uint16_t raw_humidity;
	static double humidity_val;
	static uint8_t state_humidity = 0;

	raw_humidity = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
	humidity_val = (temperature - 25) * (0.01 + 0.00008 * raw_humidity)
						- 2.0468 + 0.0367 * raw_humidity - 1.5955E-6 * pow(raw_humidity, 2);

	//limit humidty to 100%, according to SHT11 data sheet
	if (humidity_val > 100)
		humidity_val = 100;

	//print_values("Humidity_ADC :", value2);
	printf("Humidity: %u.%u\n", (int)humidity_val, (int)(100* humidity_val)%100);

//------------------------------HUMIDITY Actuators------------------------ //
	if(state_humidity != COMMAND_TYPE_HUMID_LOW 
		&& raw_humidity < THRESHOLD_HUMID_LOW) {
		return state_humidity = COMMAND_TYPE_HUMID_LOW;
	}
	else if (state_humidity != COMMAND_TYPE_HUMID_HIGH
		&& raw_humidity > THRESHOLD_HUMID_HIGH) {
		return state_humidity = COMMAND_TYPE_HUMID_HIGH;
	}
	else if (state_humidity != COMMAND_TYPE_HUMID_OK
		&& raw_humidity <= THRESHOLD_HUMID_HIGH - OFFSET_HUMIDITY 
		&& raw_humidity >= THRESHOLD_HUMID_LOW + OFFSET_HUMIDITY){
		return state_humidity = COMMAND_TYPE_HUMID_OK;
	}

	return 0;
}

uint8_t measure_temperature()
{
	static uint16_t raw_temp;
	static uint8_t state_temp = 0;
	static int temp2;

	raw_temp = sht11_sensor.value(SHT11_SENSOR_TEMP);
	temperature = 0.01 * raw_temp - 39.6;

	temp2 = (int)(temperature * -100) % 100;
	if (temp2 < 0)
		temp2 *= -1;

	printf("Temperature °C: %d.%d\n", (int)temperature, temp2);

//------------------------------Temperature Actuators------------------------ //
	if(state_temp != COMMAND_TYPE_TEMP_LOW
		&& temperature < THRESHOLD_TEMP_LOW ){
		return state_temp = COMMAND_TYPE_TEMP_LOW;
	}
	else if (state_temp != COMMAND_TYPE_TEMP_HIGH
		&& temperature > THRESHOLD_TEMP_HIGH ){
		return state_temp = COMMAND_TYPE_TEMP_HIGH;
	}
	else if (state_temp != COMMAND_TYPE_TEMP_OK
		&& temperature <= THRESHOLD_TEMP_HIGH - OFFSET_TEMP
		&& temperature >= THRESHOLD_TEMP_LOW + OFFSET_TEMP) {
		return state_temp = COMMAND_TYPE_TEMP_OK;
	}

	return 0;
}
