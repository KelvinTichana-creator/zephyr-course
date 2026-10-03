#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/printk.h>

#include "led_sensor.h"

static const struct device *sensor_dev = DEVICE_DT_GET_ANY(kelvin_led_sensor);

static int cmd_sensor_fetch(const struct shell *sh,
                            size_t argc,
                            char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (sensor_dev == NULL) {
        shell_error(sh, "LED sensor device not found");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(sensor_dev);

    if (ret < 0) {
        shell_error(sh, "sensor_sample_fetch() failed: %d", ret);
        return ret;
    }

    shell_print(sh, "sensor_sample_fetch(): LED ON");

    return 0;
}

static int cmd_sensor_read(const struct shell *sh,
                           size_t argc,
                           char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (sensor_dev == NULL) {
        shell_error(sh, "LED sensor device not found");
        return -ENODEV;
    }

    struct sensor_value val;

    int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ALL, &val);

    if (ret < 0) {
        shell_error(sh, "sensor_channel_get() failed: %d", ret);
        return ret;
    }

    shell_print(sh, "sensor_channel_get(): LED OFF");
    shell_print(sh, "Value: %d.%06d", val.val1, val.val2);

    return 0;
}

static int cmd_sensor_info(const struct shell *sh,
                           size_t argc,
                           char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (sensor_dev == NULL) {
        shell_error(sh, "LED sensor device not found");
        return -ENODEV;
    }

    shell_print(sh, "Device: %s", sensor_dev->name);
    shell_print(sh, "Ready: %s",
                device_is_ready(sensor_dev) ? "yes" : "no");

    return 0;
}

static int cmd_sensor_set(const struct shell *sh,
                          size_t argc,
                          char **argv)
{
    ARG_UNUSED(argc);

    if (sensor_dev == NULL) {
        shell_error(sh, "LED sensor device not found");
        return -ENODEV;
    }

    char *endptr;
    long value = strtol(argv[1], &endptr, 10);

    if (*argv[1] == '\0' || *endptr != '\0') {
        shell_error(sh, "value must be 0 or 1");
        return -EINVAL;
    }

    if (value < 0 || value > 1) {
        shell_error(sh, "value out of range: use 0 or 1");
        return -ERANGE;
    }

    int ret = led_sensor_set_state(sensor_dev, value != 0);

    if (ret < 0) {
        shell_error(sh, "led_sensor_set_state() failed: %d", ret);
        return ret;
    }

    shell_print(sh, "LED state set to %ld", value);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_cmds,
    SHELL_CMD(fetch,
              NULL,
              "Call sensor_sample_fetch()",
              cmd_sensor_fetch),
    SHELL_CMD(read,
              NULL,
              "Call sensor_channel_get()",
              cmd_sensor_read),
    SHELL_CMD(info,
              NULL,
              "Show sensor device information",
              cmd_sensor_info),
    SHELL_CMD_ARG(set,
                  NULL,
                  "Set LED state: 0 = OFF, 1 = ON",
                  cmd_sensor_set,
                  2,
                  0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor,
                   &sensor_cmds,
                   "LED sensor commands",
                   NULL);

int main(void)
{
    if (sensor_dev == NULL) {
        printk("LED sensor device not found\n");
        return 0;
    }

    if (!device_is_ready(sensor_dev)) {
        printk("LED sensor device is not ready\n");
        return 0;
    }

    printk("L7 Task 2: LED sensor shell ready\n");
    printk("Commands: sensor fetch, sensor read, sensor info, sensor set <0|1>\n");

    return 0;
}
