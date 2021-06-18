#ifndef LOCAL_DEVICES_H
#define LOCAL_DEVICES_H

#include <iostream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#define CL_TARGET_OPENCL_VERSION 220
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#endif

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
