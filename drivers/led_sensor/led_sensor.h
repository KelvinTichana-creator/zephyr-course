#ifndef LED_SENSOR_H
#define LED_SENSOR_H

#include <zephyr/device.h>
#include <stdbool.h>

int led_sensor_set_state(const struct device *dev, bool state);

#endif
