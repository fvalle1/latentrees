#include "LocalDevices.h"

LocalDevices* LocalDevices::devicesptr = nullptr;
cl_platform_id*  LocalDevices::fPlatforms = nullptr;
cl_device_id*  LocalDevices::fDevices = nullptr;

 auto LocalDevices::Init()->LocalDevices* {
  if(devicesptr) return devicesptr;
  else{
    devicesptr = new LocalDevices();
    return devicesptr;
  }
}

auto LocalDevices::show_devices()->LocalDevices* const{
  if(!devicesptr) Init();

  cl_uint n_platforms = 0;
  clGetPlatformIDs(1,fPlatforms,&n_platforms);
  std::cout<<"n_platforms:\t"<<n_platforms<<std::endl;

  cl_uint n_devices = 0;

  clGetDeviceIDs(fPlatforms[0], CL_DEVICE_TYPE_GPU, 5, fDevices, &n_devices);

  std::cout<<"I found:\t"<<n_devices<<" devices"<<std::endl;

  auto dev_name = new char[100];
  for (uint8_t idevice=0;idevice<n_devices;idevice++){
    size_t param_size = 0;
    clGetDeviceInfo(fDevices[0], CL_DEVICE_NAME, 100, dev_name, &param_size);
    std::cout<<dev_name<<std::endl;
  }

  return Init();
}
