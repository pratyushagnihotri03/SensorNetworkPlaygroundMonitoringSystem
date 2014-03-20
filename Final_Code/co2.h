#ifndef __CO2_H__
#define __CO2_H__

#include <stdio.h>
#include "contiki.h"
#include "settings.h"
#include "dev/sky-sensors.h"
#include "ds1000-sensor.h"

void measure_co2(uint8_t state_co2[2]);

#endif //__CO2_H__
