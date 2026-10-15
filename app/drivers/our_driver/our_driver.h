#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

void our_driver_set_intensity(const struct device *dev, int intensity);

#ifdef __cplusplus
}
#endif

#endif