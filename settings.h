#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define RIGHT 0
#define LEFT 1

/* set node IDs here */
// RIGHT = peperomia
#define ID_SINK_R 3001
#define ID_MOIST_R 3003

// LEFT = kalanchoe
#define ID_SINK_L 3008
#define ID_MOIST_L 3007

#define ID_LIGHT 3009
#define ID_CO2 3002


/* set threshold values here */
//RIGHT = peperomia
const double THRESHOLD_CO2_HIGH[] = { 950, 950 };
const double THRESHOLD_CO2_LOW[] = { 40, 40 };
const uint32_t THRESHOLD_LIGHT[] = { 680, 1200 };
const double THRESHOLD_TEMP_HIGH[] = { 28, 29 };
const double THRESHOLD_TEMP_LOW[] = { 24, 12.7 };
const uint16_t THRESHOLD_MOIS_LOW[] = { 1774, 1502 };
const uint16_t THRESHOLD_MOIS_HIGH[] = { 2428, 1917 };
const uint16_t THRESHOLD_HUMID_LOW[] = { 20, 20 };
const uint16_t THRESHOLD_HUMID_HIGH[] = { 40, 40 };

//offset values for debouncing
#define OFFSET_MOIST 100
#define OFFSET_HUMIDITY 3
#define OFFSET_LIGHT 40
#define OFFSET_TEMP 0
#define OFFSET_CO2 10

#define MEASURING_PERIOD 5 //seconds


/* unicast packet ptructure */ 
struct my_packet{
	uint8_t type;
};

enum {
//TEMP
  COMMAND_TYPE_TEMP_LOW,
  COMMAND_TYPE_TEMP_OK,
  COMMAND_TYPE_TEMP_HIGH,
//CO2
  COMMAND_TYPE_CO2_LOW,
  COMMAND_TYPE_CO2_OK,
  COMMAND_TYPE_CO2_HIGH,
//LIGHT
  COMMAND_TYPE_LIGHT_LOW,
  COMMAND_TYPE_LIGHT_OK,
//HUMID
  COMMAND_TYPE_HUMID_LOW,
  COMMAND_TYPE_HUMID_OK,
  COMMAND_TYPE_HUMID_HIGH,
//MOSITURE
  COMMAND_TYPE_MOIS_LOW,
  COMMAND_TYPE_MOIS_OK,
  COMMAND_TYPE_MOIS_HIGH
};

#endif /* __SETTINGS_H__ */
