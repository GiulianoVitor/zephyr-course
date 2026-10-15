#define DT_DRV_COMPAT zephyr_our_driver

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "our_driver.h"

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

#define LED_NODE DT_ALIAS(ledr)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

//Our driver data structure
struct our_driver_data
{
    int intensity;
};

static struct our_driver_data our_driver_data0;

//Driver Init
static int our_driver_init(const struct device *dev)
{
    if (!gpio_is_ready_dt(&led))
    {
        LOG_ERR("GPIO for led is not ready!");
        return -ENODEV;
    }
    //Configure GPIO pin
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    LOG_INF("OUR DRIVER initialized");

    return 0;
}

//Fetch function must activate LED
static int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    LOG_INF("Our Driver - fetch");
    return gpio_pin_set_dt(&led, 1);
}

//Get function must Deactivate LED
static int our_driver_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    LOG_INF("Our Driver - get");
    return gpio_pin_set_dt(&led, 0);
}

void our_driver_set_intensity(const struct device *dev, int intensity)
{
    struct our_driver_data *data = dev->data;
    data->intensity = intensity;
    LOG_INF("Driver Intensity = %d", intensity);
}

//Bind functions with Sensor API
static DEVICE_API(sensor, our_driver_api) = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get  = our_driver_channel_get,
};


DEVICE_DT_INST_DEFINE(
    0, 
    our_driver_init, 
    NULL, 
    &our_driver_data0, 
    NULL, 
    POST_KERNEL, 
    80, 
    &our_driver_api
);