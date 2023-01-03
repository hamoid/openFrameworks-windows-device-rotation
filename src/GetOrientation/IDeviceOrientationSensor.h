#pragma once

#include <windows.h>
#include <sensors.h>
#include <sensorsapi.h>
#include <string>

enum class DeviceOrientation {
    GetError = -1,
    NotRotated = 0,
    Rotated90DegreesCounterclockwise = 1,
    Rotated180DegreesCounterclockwise = 2,
    Rotated270DegreesCounterclockwise = 3,
    Faceup = 4,
    Facedown = 5
};


class IDeviceOrientationSensor {
public:
    IDeviceOrientationSensor(ISensor* Sensor,  const PROPERTYKEY propertykey) {
        _pSensor = Sensor;
        _propertykey = propertykey;
    };
    ~IDeviceOrientationSensor() {
        if (_pSensor != nullptr) {
            _pSensor->Release();
            _pSensor = nullptr;
        }
    };
    virtual DeviceOrientation getOrientation() = 0;

protected:
    ISensor* _pSensor = nullptr;
    PROPERTYKEY _propertykey;
    HRESULT getReport(PROPVARIANT& pvTilt) {
        ISensorDataReport* pReport = nullptr;
        if (_pSensor == nullptr) {
            return E_POINTER;
        }
        auto hr = _pSensor->GetData(&pReport);
        if (FAILED(hr)) {
            OutputDebugString(L"Unable to _pSensor GetData() ");
            return hr;
        }
        hr = pReport->GetSensorValue(_propertykey, &pvTilt);
        if (FAILED(hr)) {
            OutputDebugString(L"Unable to _pSensor GetSensorValue() ");
            return hr;
        }

        if (pReport != nullptr) {
            pReport->Release();
        }

        return hr;
    };
};

