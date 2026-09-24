#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "led_sensor.h"

int main(void)
{
    const struct device *dev = DEVICE_DT_GET_ANY(kelvin_led_sensor);
    struct sensor_value val;
    int ret;

    if (dev == NULL) {
        printk("LED sensor device not found\n");
        return 0;
    }

    if (!device_is_ready(dev)) {
        printk("LED sensor device is not ready\n");
        return 0;
    }

    printk("L6 Task 2: LED sensor ready\n");

    /* Custom extension API */
    ret = led_sensor_set_state(dev, true);
    if (ret < 0) {
        printk("led_sensor_set_state() failed: %d\n", ret);
        return 0;
    }

    printk("Custom API: LED ON\n");

    while (1) {
        ret = sensor_sample_fetch(dev);
        if (ret < 0) {
            printk("sensor_sample_fetch() failed: %d\n", ret);
        } else {
            printk("sensor_sample_fetch(): LED ON\n");
        }

        k_msleep(1000);

        ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
        if (ret < 0) {
            printk("sensor_channel_get() failed: %d\n", ret);
        } else {
            printk("sensor_channel_get(): LED OFF\n");
        }

        k_msleep(1000);
    }

    return 0;
}
