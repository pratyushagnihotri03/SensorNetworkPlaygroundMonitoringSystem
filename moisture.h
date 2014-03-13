#ifndef __MOISTURE_H__
#define __MOISTURE_H__

#include <stdio.h>
#include "contiki.h"
#include "settings.h"
#include "dev/sky-sensors.h"
#include "vh400.h"

void measure_moisture(uint16_t my_id);

#endif //__MOISTURE_H__
