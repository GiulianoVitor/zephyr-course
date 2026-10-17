#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <errno.h>
#include <stdlib.h>
#include "our_driver.h"

#define OUR_DRIVER_NODE DT_NODELABEL(our_driver0)
static const struct device *mySensor = DEVICE_DT_GET(OUR_DRIVER_NODE);

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    if (!device_is_ready(mySensor)) {
        shell_error(sh, "Sensor not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(mySensor);

    if (ret < 0) {
        shell_error(sh, "Fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Fetch command executed: LED ON");
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
    if (!device_is_ready(mySensor)) {
        shell_error(sh, "Sensor not ready");
        return -ENODEV;
    }

    struct sensor_value val = {0};

    int ret = sensor_channel_get(mySensor, SENSOR_CHAN_ALL, &val);

    if (ret < 0) {
        shell_error(sh, "Read failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Read command executed: LED OFF");
    return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Device: %s", mySensor->name);
    shell_print(sh, "Ready: %s", device_is_ready(mySensor) ? "Yes" : "No");
    return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
    char *endptr;
    long intensity;

    //validate the arguments
    if(argc != 2)
    {
        shell_error(sh, "Usage: sensor set <0-100>");
        return -ENODEV;
    }

     // Convert string to integer
    errno = 0;
    intensity = strtol(argv[1], &endptr, 10);

    // Validate conversion and range
    if (errno == ERANGE ||
        endptr == argv[1] ||
        *endptr != '\0' ||
        intensity < 0 ||
        intensity > 100) {

        shell_error(sh, "Intensity must be an integer from 0 to 100");
        return -EINVAL;
    }

    // Check device readiness
    if (!device_is_ready(mySensor)) {
        shell_error(sh, "Sensor not ready");
        return -ENODEV;
    }

    // Call custom driver API
    our_driver_set_intensity(mySensor, (int)intensity);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor_cmds,
    SHELL_CMD(fetch, NULL, "Calls sensor_sample_fetch()", cmd_sensor_fetch),
    SHELL_CMD(info, NULL, "Prints the device name and ready state", cmd_sensor_info),
    SHELL_CMD(read, NULL, "Calls sensor_channel_get() and prints result", cmd_sensor_read),
    SHELL_CMD_ARG(set, NULL, "Set sensor intensity. Usage: sensor set <value>", cmd_sensor_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor_cmds, "Sensor root shell command", NULL);