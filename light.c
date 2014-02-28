#include "light.h"

uint8_t measure_light()
{
	static uint16_t raw_light;
	
	raw_light = light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
	printf("Light_Sensor_ADC: %u\n", raw_light);


//------------------------------Light Actuators------------------------ //

	if(raw_light < THRESHOLD_LIGHT ){
		return COMMAND_TYPE_LIGHT_LOW;
	}
	else {
		return COMMAND_TYPE_LIGHT_OK;
	}

	return 0;
}

uint8_t measure_humidity()
{
	uint8_t raw_humidity;

	raw_humidity = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
	double humidity_val= (-4.0 + 405.0 * raw_humidity / 10000);

	//print_values("Humidity_ADC :", value2);
	printf("Humidity: %u.%u\n", (int)humidity_val, (int)(100* humidity_val)%100);

//------------------------------HUMIDITY Actuators------------------------ //
	if(raw_humidity < THRESHOLD_HUMID_LOW ){
		return COMMAND_TYPE_HUMID_LOW;
	}
	else if (raw_humidity > THRESHOLD_HUMID_HIGH ){
		return COMMAND_TYPE_HUMID_HIGH;
	}
	else if (raw_humidity <=THRESHOLD_HUMID_HIGH   && raw_humidity >= THRESHOLD_HUMID_LOW){
		return COMMAND_TYPE_HUMID_OK;
	}

	return 0;
}

uint8_t measure_temperature()
{
	uint8_t raw_temp;
	int temperature;

	raw_temp = sht11_sensor.value(SHT11_SENSOR_TEMP);
	temperature = ((0.01 * raw_temp -32)* .55);

	//print_values("Temperature_ADC :", value3);
	printf("Temperature °C: %d\n", temperature);

//------------------------------Temperature Actuators------------------------ //
	if(raw_temp < THRESHOLD_TEMP_LOW ){
		return COMMAND_TYPE_TEMP_LOW;
	}
	else if (raw_temp > THRESHOLD_TEMP_HIGH ){
		return COMMAND_TYPE_TEMP_HIGH;
	}
	else if (raw_temp <=THRESHOLD_TEMP_HIGH   && raw_temp >= THRESHOLD_TEMP_LOW ){
		return COMMAND_TYPE_TEMP_OK;
	}

	return 0;
}
