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


/* set threshold values for {RIGHT, LEFT} plant in main.c */
extern const double THRESHOLD_CO2_HIGH[];
extern const double THRESHOLD_CO2_LOW[];
extern const uint32_t THRESHOLD_LIGHT[];
extern const double THRESHOLD_TEMP_HIGH[];
extern const double THRESHOLD_TEMP_LOW[];
extern const uint16_t THRESHOLD_MOIS_LOW[];
extern const uint16_t THRESHOLD_MOIS_HIGH[];
extern const uint16_t THRESHOLD_HUMID_LOW[];
extern const uint16_t THRESHOLD_HUMID_HIGH[];

extern const char * plant_name[];

//offset values for debouncing
#define OFFSET_MOIST 0//100
#define OFFSET_HUMIDITY 0//3
#define OFFSET_LIGHT 0//20
#define OFFSET_TEMP 0
#define OFFSET_CO2 0//10

#define MEASURING_PERIOD 5 //seconds
#define SENDING_DELAY 1 //seconds

/* unicast packet ptructure */ 
struct my_packet{
	uint8_t type;
};

enum {
//TEMP
  TEMP_LOW,
  TEMP_OK,
  TEMP_HIGH,
//CO2
  CO2_LOW,
  CO2_OK,
  CO2_HIGH,
//LIGHT
  LIGHT_LOW,
  LIGHT_OK,
//HUMID
  HUMID_LOW,
  HUMID_OK,
  HUMID_HIGH,
//MOSITURE
  MOIS_LOW,
  MOIS_OK,
  MOIS_HIGH
};

#endif /* __SETTINGS_H__ */
