#include "moisture.h"

void measure_moisture(uint8_t cmd[2], uint16_t my_id)
{
	static uint16_t raw;
	static double voltage, moisture;
	static uint8_t plant;
        static uint8_t state_moisture[2];

	raw = vh400.value(ADC3);
	voltage = 3 * raw / 4096.0;

	if (voltage < 1.1)
		moisture = 10 * voltage - 1;
	else if (voltage < 1.3)
		moisture = 25 * voltage - 17.5;
	else if (voltage < 1.82)
		moisture = 48.08 * voltage - 47.5;
	else
		moisture = 26.32 * voltage - 7.89;

	if (my_id == ID_MOIST_R) {
		printf ("Peperomia ");
		plant = RIGHT;
	}
	else {
		printf ("Kalanchoe ");
		plant = LEFT;
	}

	printf ("Soil Moisture: %u.%u\n", (int)moisture, (int)(moisture * 100) % 100);

	state_moisture[RIGHT] = 0;
	state_moisture[LEFT] = 0;

//------------------------------MOISTURE Actuators------------------------ //
	if(state_moisture[plant] != COMMAND_TYPE_MOIS_LOW && raw < THRESHOLD_MOIS_LOW[plant]) {
		state_moisture[plant] = 0;
		printf("MOISTURE LOW!\n");
		return;
	}
	else if (state_moisture[plant] != COMMAND_TYPE_MOIS_HIGH && raw > THRESHOLD_MOIS_HIGH[plant]) {
		state_moisture[plant] = COMMAND_TYPE_MOIS_HIGH;
		return;
	}
	else if (state_moisture[plant] != COMMAND_TYPE_MOIS_OK
			&& raw <= THRESHOLD_MOIS_HIGH[plant] - OFFSET_MOIST
			&& raw >= THRESHOLD_MOIS_LOW[plant] + OFFSET_MOIST){
		state_moisture[plant] = COMMAND_TYPE_MOIS_OK;
		return;
	}

	return;
}
