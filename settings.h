#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define RIGHT //peperomia
//#define LEFT //kalanchoe

/* set node IDs here */
#define ID_LIGHT 3009
#define ID_CO2 3002

#ifdef RIGHT //peperomia
#define ID_SINK 3001
#define ID_MOIST 3003
#endif //RIGHT

#ifdef LEFT //kalanchoe
#define ID_SINK 3008
#define ID_MOIST 3007
#endif //LEFT

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

#ifdef RIGHT //kalanchoe
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


//offset values for debouncing
#define OFFSET_MOIST 100

#endif /* __SETTINGS_H__ */
