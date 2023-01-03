#pragma once

#include "IDeviceOrientationSensor.h"

class DeviceOrientationSensor : public IDeviceOrientationSensor {
public:
    DeviceOrientationSensor(ISensor* Sensor, const PROPERTYKEY propertykey) ;
    DeviceOrientation getOrientation();
};

