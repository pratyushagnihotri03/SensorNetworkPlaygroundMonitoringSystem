#include "moisture.h"

uint8_t measure_moisture()
{
	static uint16_t raw;
	static double voltage, moisture;
	static uint8_t state = 0;

	raw = vh400.value(ADC0);
	voltage = 3 * raw / 4096.0;

	if (voltage < 1.1)
		moisture = 10 * voltage - 1;
	else if (voltage < 1.3)
		moisture = 25 * voltage - 17.5;
	else if (voltage < 1.82)
		moisture = 48.08 * voltage - 47.5;
	else
		moisture = 26.32 * voltage - 7.89;

	printf ("Soil Moisture: %u.%u\n", (int)moisture, (int)(moisture * 100) % 100);

//------------------------------MOISTURE Actuators------------------------ //
	if(state != COMMAND_TYPE_MOIS_LOW && raw < THRESHOLD_MOIS_LOW) {
		return state = COMMAND_TYPE_MOIS_LOW;
	}
	else if (state != COMMAND_TYPE_MOIS_HIGH && raw > THRESHOLD_MOIS_HIGH) {
		return state = COMMAND_TYPE_MOIS_HIGH;
	}
	else if (state != COMMAND_TYPE_MOIS_OK
			&& raw <= THRESHOLD_MOIS_HIGH - OFFSET_MOIST
			&& raw >= THRESHOLD_MOIS_LOW + OFFSET_MOIST){
		return state = COMMAND_TYPE_MOIS_OK;
	}
	return 0;
}
