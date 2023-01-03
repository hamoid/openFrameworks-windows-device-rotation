#include "DeviceOrientationSensor.h"
#include "Quaternion.hpp"

const DeviceOrientation orientationsArray[] = {
  DeviceOrientation::Rotated90DegreesCounterclockwise,   // device x axis points up/down
  DeviceOrientation::Rotated270DegreesCounterclockwise ,  
  DeviceOrientation::NotRotated,                         // device y axis points up/down
  DeviceOrientation::Rotated180DegreesCounterclockwise,   
  DeviceOrientation::Faceup,                             // device z axis points up/down
  DeviceOrientation::Facedown 
};      

DeviceOrientationSensor::DeviceOrientationSensor(ISensor* Sensor, const PROPERTYKEY propertykey) :
    IDeviceOrientationSensor(Sensor, propertykey) { }


DeviceOrientation DeviceOrientationSensor::getOrientation() {
    PROPVARIANT pvTilt;
    auto hr = getReport(pvTilt);
    if (FAILED(hr)) {
        return DeviceOrientation::GetError;
    }
    
    auto p_element = (float*)pvTilt.caub.pElems;
    auto x = p_element[0];
    auto y = p_element[1];
    auto z = p_element[2];
    auto w = p_element[3];

    Quaternion quat(x, y, z, w);
    Quaternion cquat = Quaternion::Conjugate(quat);
    auto vec = Quaternion::FromToRotation(Quaternion::ToEuler(cquat), Vector3(0, 0, 1));

    int maxIndex = -1;
    double maxValue = -1;
    for (int i = 0; i < 3; i++) {
        if (maxValue < abs(vec.data[i])) {
            maxIndex = i;
            maxValue = abs(vec.data[i]);
        }
    }
    if (vec.data[maxIndex] > 0) {
        maxIndex = maxIndex * 2;
    } else {
        maxIndex = maxIndex * 2 + 1;
    }

    PropVariantClear(&pvTilt);
    return orientationsArray[maxIndex];
}

