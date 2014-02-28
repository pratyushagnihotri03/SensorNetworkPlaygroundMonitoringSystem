#include "light.h"

uint8_t measure_light()
{
	static uint16_t raw_light;
	static uint8_t state_light = 0;
	
	raw_light = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
	printf("Light_Sensor_ADC: %u\n", raw_light);


//------------------------------Light Actuators------------------------ //

	if(state_light != COMMAND_TYPE_LIGHT_LOW && raw_light < THRESHOLD_LIGHT){
		return COMMAND_TYPE_LIGHT_LOW;
	}
	else if (state_light != COMMAND_TYPE_LIGHT_OK 
		&& raw_light >= THRESHOLD_LIGHT + OFFSET_LIGHT) {
		return COMMAND_TYPE_LIGHT_OK;
	}

	return 0;
}

uint8_t measure_humidity()
{
	static uint8_t raw_humidity;
	static double humidity_val;
	static uint8_t state_humidity = 0;

	raw_humidity = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
	humidity_val= (-4.0 + 405.0 * raw_humidity / 10000);

	//print_values("Humidity_ADC :", value2);
	printf("Humidity: %u.%u\n", (int)humidity_val, (int)(100* humidity_val)%100);

//------------------------------HUMIDITY Actuators------------------------ //
	if(state_humidity != COMMAND_TYPE_HUMID_LOW 
		&& raw_humidity < THRESHOLD_HUMID_LOW) {
		return COMMAND_TYPE_HUMID_LOW;
	}
	else if (state_humidity != COMMAND_TYPE_HUMID_HIGH
		&& raw_humidity > THRESHOLD_HUMID_HIGH) {
		return COMMAND_TYPE_HUMID_HIGH;
	}
	else if (state_humidity != COMMAND_TYPE_HUMID_OK
		&& raw_humidity <= THRESHOLD_HUMID_HIGH - OFFSET_HUMIDITY 
		&& raw_humidity >= THRESHOLD_HUMID_LOW + OFFSET_HUMIDITY){
		return COMMAND_TYPE_HUMID_OK;
	}

	return 0;
}

uint8_t measure_temperature()
{
	static uint8_t raw_temp;
	static int temperature;
	static uint8_t state_temp = 0;

	raw_temp = sht11_sensor.value(SHT11_SENSOR_TEMP);
	temperature = ((0.01 * raw_temp -32)* .55);

	//print_values("Temperature_ADC :", value3);
	printf("Temperature °C: %d\n", temperature);

//------------------------------Temperature Actuators------------------------ //
	if(state_temp != COMMAND_TYPE_TEMP_LOW
		&& raw_temp < THRESHOLD_TEMP_LOW ){
		return COMMAND_TYPE_TEMP_LOW;
	}
	else if (state_temp != COMMAND_TYPE_TEMP_HIGH
		&& raw_temp > THRESHOLD_TEMP_HIGH ){
		return COMMAND_TYPE_TEMP_HIGH;
	}
	else if (raw_temp <=THRESHOLD_TEMP_HIGH   && raw_temp >= THRESHOLD_TEMP_LOW ){
		return COMMAND_TYPE_TEMP_OK;
	}

	return 0;
}
