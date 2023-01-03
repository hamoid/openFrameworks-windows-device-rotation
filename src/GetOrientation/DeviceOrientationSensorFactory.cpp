#include "DeviceOrientationSensorFactory.h"
#include "DeviceOrientationSensor.h"
#include <sensorsapi.h>

template <class T> void SafeRelease(T** ppT) {
    if (*ppT) {
        (*ppT)->Release();
        *ppT = nullptr;
    }
}

IDeviceOrientationSensor* DeviceOrientationSensorFactory::FindAndCreatDeviceOrientationSensorInstance() {
    IDeviceOrientationSensor* pIDeviceOrientationSensor = nullptr;
    CoInitialize(nullptr);
    ISensorCollection* pSensorColl = nullptr;
    ISensorManager* pSensorManager = nullptr;
    ISensor* pSensor = nullptr;
    HRESULT hr = ::CoCreateInstance(CLSID_SensorManager, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSensorManager));
    
    do {
        if (FAILED(hr)) {
            OutputDebugString(L"CoCreateInstance error");
            break;
        }

        hr = pSensorManager->GetSensorsByType(SENSOR_TYPE_AGGREGATED_DEVICE_ORIENTATION, &pSensorColl);

        if (SUCCEEDED(hr)) {
            hr = pSensorColl->GetAt(0, &pSensor);
            pIDeviceOrientationSensor = new DeviceOrientationSensor(pSensor, SENSOR_DATA_TYPE_QUATERNION);
            break;
        }

        if (FAILED(hr)) {
            OutputDebugString(L"No Sensor");
        }

    } while (false);

    if (pSensor != nullptr && pIDeviceOrientationSensor != nullptr) {

        ISensorCollection* pSensorCollection = NULL;
        HRESULT hr = ::CoCreateInstance(CLSID_SensorCollection, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSensorCollection));
        do {
            if (FAILED(hr)) {
                OutputDebugString(L"Unable to CoCreateInstance() a SensorCollection.");
                break;
            }
            pSensorCollection->Clear();
            pSensorCollection->Add(pSensor);
            // Have the SensorManager prompt the end-user for permission.
            hr = pSensorManager->RequestPermissions(NULL, pSensorCollection, TRUE);
            SafeRelease(&pSensorCollection);
            if (FAILED(hr)) {
                OutputDebugString(L"No permission to access Requested Sensor.");
                if (pIDeviceOrientationSensor != nullptr) {
                    delete pIDeviceOrientationSensor;
                    pIDeviceOrientationSensor = nullptr;
                }
                break;
            }
        } while (false);

    }
    
    SafeRelease(&pSensorColl);
    SafeRelease(&pSensorManager);

    return pIDeviceOrientationSensor;
}

void DeviceOrientationSensorFactory::Release(IDeviceOrientationSensor* instance) {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
    CoUninitialize();
}

