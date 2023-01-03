#include <iostream>
#include "GetOrientation/DeviceOrientationSensorFactory.h"

//#include "ofApp.h"
//#include "ofAppRunner.h"
//#include "ofWindowSettings.h"
//int main( ) {
//    ofSetupOpenGL(1024, 768, OF_FULLSCREEN);
//    ofRunApp(new ofApp());
//}

int main() {
    std::cout << "Start\n";

    auto sensor = DeviceOrientationSensorFactory::FindAndCreatDeviceOrientationSensorInstance();
    if (sensor == nullptr) {
        std::cout << "There is NO device orientation sensor or need running as admin\n";
    } else {
        int i = 0;
        do {
            if(i++ > 10) {
                break;
            }
            auto res = sensor->getOrientation();
            if (res == DeviceOrientation::GetError) {
                std::cout << "Got error when getting sensor data\n";
                break;
            }
            std::cout << "Device orientation: " << (int)res << std::endl;
            Sleep(2000);

        } while (true);
    }

    DeviceOrientationSensorFactory::Release(sensor);
}
