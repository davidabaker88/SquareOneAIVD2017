#ifndef LIBRARY_H
#define LIBRARY_H

// Custom Libraries
#include "./vesc/VescUart.h"
#include "./gyro/gyro.h"
#include "./encoder_driver/encoder_driver.h"

// Maybe some constants like pid tuning values
#define STEERING_P 1.0
#define STEERING_I 0.0
#define STEERING_D 0.0

#endif // LIBRARY_H