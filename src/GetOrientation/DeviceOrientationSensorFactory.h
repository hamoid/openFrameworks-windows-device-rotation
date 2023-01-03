#pragma once

#include "IDeviceOrientationSensor.h"

class DeviceOrientationSensorFactory {
public:
    static IDeviceOrientationSensor* FindAndCreatDeviceOrientationSensorInstance();
    static void Release(IDeviceOrientationSensor * instance);
};

