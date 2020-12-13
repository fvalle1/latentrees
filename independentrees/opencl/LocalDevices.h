#ifndef LOCAL_DEVICES_H
#define LOCAL_DEVICES_H

#include <iostream>
#include <OpenCL/opencl.h>

using std::cout;
using std::endl;

class LocalDevices{
public:
  static auto Init()->LocalDevices*;

    static auto Destroy(){
      if (devicesptr) {
        delete fDevices;
        delete fPlatforms;
        delete devicesptr;
      }
    }

    static auto show_devices()->LocalDevices* const;

    static auto get_devices(){
      return fDevices;
    }

  private:

    LocalDevices(){
      fDevices = new cl_device_id[5];
      fPlatforms = new cl_platform_id();
    }

    static LocalDevices* devicesptr;

    static cl_platform_id*  fPlatforms;
    static cl_device_id*  fDevices;
  };

  #endif
