#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stdio.h>
#include "contiki.h"
#include "settings.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"
#include "dev/sht11-sensor.h"
#include <math.h>

void measure_light(uint8_t state_light[2]);
void measure_temperature();
void measure_humidity();

#endif //__LIGHT_H__
