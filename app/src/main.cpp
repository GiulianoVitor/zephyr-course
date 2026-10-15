#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include "drivers/our_driver/our_driver.h"

#define OUR_DRIVER_NODE DT_NODELABEL(our_driver0)
static const struct device *mySensor = DEVICE_DT_GET(OUR_DRIVER_NODE);

int main(void)
{
    int i=0;
    if (!device_is_ready(mySensor)) 
    {
        printk("Our driver is not ready!\n");
        return 0;
    }

    printk("Our driver is ready!\n");

    while (1) {
        
        sensor_sample_fetch(mySensor);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        struct sensor_value dummyVal;
        sensor_channel_get(mySensor, SENSOR_CHAN_ALL, &dummyVal);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        our_driver_set_intensity(mySensor, i++);

        printk("Heartbeat !\n");
    }
    return 0;
}
