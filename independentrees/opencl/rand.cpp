//clang++ -std=c++17 -O3 -Ipcg-cpp/include -L. -framework OpenCL matmul.cpp LocalDevices.o -o matmul.out

#include <iostream>
#include <fstream>
#include <random>
#include <numeric>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include "pcg_random.hpp"

#include "LocalDevices.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#define CL_TARGET_OPENCL_VERSION 220
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#endif

using std::cout;
using std::endl;

std::mutex m;

template <typename T>
void streamToFile(std::queue<T> &buffer, const uint32_t &dataCount, std::ofstream &dataFile, const bool &ended, const bool &pushing){
  //std::cout<<"Thread "<<std::this_thread::get_id()<<std::endl;
  // read the data
  m.lock();
  int written = 0;
  while (!ended)
  {
    if (pushing)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(1));
      continue;
    }
    if (buffer.size() > 0)
    {
      written++;
      for (int i = 0; i < dataCount; i++)
        dataFile << buffer.front()[i] << std::endl;
      delete[] buffer.front();
      buffer.pop();
    }
  }

  auto remaining = buffer.size();
  std::cout << "I wrote " << written << " statistics" << std::endl;
  std::cout << "have to write " << remaining << " more" << std::endl;
  for (int e = 0; e < remaining; e++)
  {
    for (int i = 0; i < dataCount; i++)
      dataFile << buffer.front()[i] << std::endl;
    delete[] buffer.front();
    buffer.pop();
  }

  m.unlock();
}

void printStatus(uint32_t &status, bool &ended)
{
  while (!ended)
  {
    std::cout << "\r" << status;
  }
  std::cout << std::endl;
}

int main(void) {
  auto devices = LocalDevices::Init()->show_devices()->get_devices();

  int err;
  cl_device_id device_id = devices[0];

  cl_context context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
  cl_command_queue commands = clCreateCommandQueue(context, device_id, 0, &err);
  if (err != 0){
    std::cerr<<"Error command queue"<<endl;
    return err;
  }

  const char* file_name = "random.cl";
  auto program = clCreateProgramWithSource(context,1, &file_name, NULL, &err);
  if (err != 0)
  {
    std::cerr << "Error clCreateProgramWithSource" << endl;
    return err;
  }

  const uint32_t DATA_SIZE = 1024;
  const uint16_t statistics = 10;

  //cl_program program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
  clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

  cl_kernel kernel = clCreateKernel(program, "storyMersenne", &err);
  if (err != 0)
  {
    std::cerr << "Error clCreateKernel" << endl;
    return err;
  }
  cl_mem input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * DATA_SIZE, NULL, NULL);
  cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * DATA_SIZE, NULL, NULL);

  float A[DATA_SIZE];

  uint32_t i = 0;
  bool ended = false;
  bool pushing = false;
  auto tPrint = std::thread(printStatus, std::ref(i), std::ref(ended));
  tPrint.detach();

  std::ofstream dataFile;
  dataFile.open("randoms.dat", std::ios::out /*| std::ios::app*/);
  if (!dataFile.is_open())
  {
    std::cerr << "File to store data not opened" << std::endl;
    return -1;
  }

  std::queue<float *> buffer;
  auto tWrite = std::thread(streamToFile<float *>, std::ref(buffer), std::ref(DATA_SIZE), std::ref(dataFile), std::ref(ended), std::ref(pushing));

  auto gpu_start = std::chrono::steady_clock::now();

  for (i = 0; i< statistics; i++){
    for (int j = 0; j < DATA_SIZE; j++){
      A[j] = j + i*DATA_SIZE;
    }
    err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(float) * DATA_SIZE, A, 0, NULL, NULL);
    if (err != 0)
      return err;

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);

    unsigned int count = DATA_SIZE;
    clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
    size_t local;
    clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    size_t global = count;
    clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(commands);

    float results[DATA_SIZE];
    clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * DATA_SIZE, results, 0, NULL, NULL);

    // read the data
    {
      
      for(int j= 0; j<DATA_SIZE;j++){
        //cout<<results[j]<<endl;
      }
      auto outData = static_cast<float *>(results);
      pushing = true;
      auto newplace = new float[DATA_SIZE];
      std::memcpy(newplace, std::move(outData), DATA_SIZE * sizeof(float));
      buffer.push(newplace);
      pushing = false;
    }
  }

  auto gpu_end = std::chrono::steady_clock::now();

  ended = true;

  if (tWrite.joinable())
  {
    std::cout << std::endl
              << "Waiting all threads to finish" << std::endl;
    tWrite.join();
  };

  auto gpu_time = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds> (gpu_end - gpu_start).count());
  std::cout << "Time (GPU[OpenCL]) = " << gpu_time << "[ms]" << std::endl;

  clReleaseMemObject(input);
  clReleaseMemObject(output);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(commands);
  clReleaseContext(context);
  LocalDevices::Destroy();
  return 0;
}
