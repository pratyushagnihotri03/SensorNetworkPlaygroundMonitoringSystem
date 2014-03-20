#include "moisture.h"

void measure_moisture(uint16_t my_id, uint8_t cmd[2])
{
	static uint32_t raw;
	static double voltage, moisture;
	static uint8_t plant, i;
        static uint8_t state_moisture[2] = {MOIS_OK, MOIS_OK};

	raw = 0;
	for (i=0; i < 3; i++)
		raw += vh400.value(ADC0);
	raw /= 3;

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

	cmd[plant] = 0;

//------------------------------MOISTURE Actuators------------------------ //
	if(state_moisture[plant] != MOIS_LOW && raw < THRESHOLD_MOIS_LOW[plant]) {
		if (state_moisture[plant] == MOIS_HIGH)
			cmd[plant] = MOIS_OK;
		state_moisture[plant] = MOIS_LOW;
		printf("%s Moisture low\n", plant_name[plant]);
		return;
	}
	else if (state_moisture[plant] != MOIS_HIGH && raw > THRESHOLD_MOIS_HIGH[plant]) {
		state_moisture[plant] = MOIS_HIGH;
		cmd[plant] = MOIS_HIGH;
		return;
	}
	else if (raw <= THRESHOLD_MOIS_HIGH[plant] - OFFSET_MOIST
			&& raw >= THRESHOLD_MOIS_LOW[plant] + OFFSET_MOIST){
		if (state_moisture[plant] == MOIS_HIGH) {
			state_moisture[plant] = MOIS_OK;
			cmd[plant] = MOIS_OK;
		}
		else if (state_moisture[plant] == MOIS_LOW) {
			state_moisture[plant] = MOIS_OK;
			printf("%s Moisture ok\n", plant_name[plant]);
		}
		return;
	}

	return;
}
