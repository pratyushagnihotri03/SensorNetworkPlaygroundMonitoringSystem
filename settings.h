#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define RIGHT //peperomia
//#define LEFT //kalanchoe
//#define COOJA
//#define TEST

/* set node IDs here */

#ifdef RIGHT //peperomia
#define ID_SINK 3001
#define ID_MOIST 3003
#define ID_LIGHT 3009
#define ID_CO2 3002
#endif //RIGHT

#ifdef LEFT //kalanchoe
#define ID_SINK 3008
#define ID_MOIST 3007
#define ID_LIGHT 3009
#define ID_CO2 3002
#endif //LEFT

#ifdef COOJA
#define ID_SINK 1
#define ID_MOIST 2
#define ID_LIGHT 3
#define ID_CO2 4
#endif //COOJA

#ifdef TEST
#define ID_SINK 1
#define ID_MOIST 2
#define ID_LIGHT 22998
#define ID_CO2 3
#define COOJA
#endif //TEST

/* set threshold values here */
#ifdef RIGHT //peperomia
#define THRESHOLD_CO2_HIGH 81
#define THRESHOLD_CO2_LOW 58
#define THRESHOLD_LIGHT 640
#define THRESHOLD_TEMP_HIGH 28 
#define THRESHOLD_TEMP_LOW 24
#define THRESHOLD_MOIS_LOW 1765 
#define THRESHOLD_MOIS_HIGH 1950
#define THRESHOLD_HUMID_LOW 20
#define THRESHOLD_HUMID_HIGH 40
#endif //RIGHT

#ifdef LEFT //kalanchoe
#define THRESHOLD_CO2_HIGH 81
#define THRESHOLD_CO2_LOW 58
#define THRESHOLD_LIGHT 640
#define THRESHOLD_TEMP_HIGH 28 
#define THRESHOLD_TEMP_LOW 24
#define THRESHOLD_MOIS_LOW 1765 
#define THRESHOLD_MOIS_HIGH 1950
#define THRESHOLD_HUMID_LOW 20
#define THRESHOLD_HUMID_HIGH 40
#endif //RIGHT

#ifdef COOJA
#define THRESHOLD_CO2_HIGH 81
#define THRESHOLD_CO2_LOW 58
#define THRESHOLD_LIGHT 640
#define THRESHOLD_TEMP_HIGH 28 
#define THRESHOLD_TEMP_LOW 24
#define THRESHOLD_MOIS_LOW 1765 
#define THRESHOLD_MOIS_HIGH 1950
#define THRESHOLD_HUMID_LOW 20
#define THRESHOLD_HUMID_HIGH 40
#endif //COOJA

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
