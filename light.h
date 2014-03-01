#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <stdio.h>
#include "contiki.h"
#include "settings.h"
#include "dev/sky-sensors.h"
#include "dev/light-sensor.h"
#include "dev/sht11-sensor.h"
#include <math.h>

uint8_t measure_light();
uint8_t measure_temperature();
uint8_t measure_humidity();

#endif //__LIGHT_H__
