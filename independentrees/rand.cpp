#include "mtlpp.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <queue>
#include <cmath>

std::mutex m;

template <typename T>
void streamToFile(std::queue<T> &buffer, const uint32_t &dataCount, std::ofstream &dataFile, const bool &ended, const bool &pushing)
{
  //std::cout<<"Thread "<<std::this_thread::get_id()<<std::endl;
  // read the data
  m.lock();
  int written = 0;
  while (!ended)
  {
    if(pushing){
      std::this_thread::sleep_for(std::chrono::nanoseconds(1));
      continue;
    }
    if(buffer.size()>0){
        written++;
        for (int i = 0; i < dataCount; i++) dataFile << buffer.front()[i] << std::endl;
        delete[] buffer.front();
        buffer.pop();
   }
  }

  auto remaining = buffer.size();
  std::cout << "I wrote " <<written <<" statistics"<< std::endl;
  std::cout << "have to write "<<remaining <<" more"<< std::endl;
  for (int e = 0; e < remaining; e++)
  {
    for (int i = 0; i < dataCount; i++) dataFile << buffer.front()[i] << std::endl;
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

int main()
{
using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();
    assert(device);

    const char shadersSrc[] = R"""(
        #include <metal_stdlib>

        #include "mersenne.metal"
        #include "random.metal"

        using namespace metal;

        kernel void storyMersenne(
            const device uint *vIn [[ buffer(0) ]],
            device int *vOut [[ buffer(1) ]],
            uint id[[ thread_position_in_grid ]]){
          
          mt19937 mers;

          mers.srand(vIn[id]);
         
          float temp = 10.0;

          for (uint l = 0; l < 100; l++){
              float delta = 1.0 + sqrt(3.0)*abs(temp);
              float r = mers.rand();
              temp = round(2*r*delta+temp-delta);
              if(temp > 1000000000000000){
              temp = 1000000000000000;
              }
          }

          vOut[id] = round(temp);
                  
        }

        
    )""";

    mtlpp::Library library = device.NewLibrary(shadersSrc, mtlpp::CompileOptions(), nullptr);
    assert(library);
    mtlpp::Function sqrFunc = library.NewFunction("storyMersenne");
    assert(sqrFunc);

    mtlpp::ComputePipelineState computePipelineState = device.NewComputePipelineState(sqrFunc, nullptr);
    assert(computePipelineState);

    mtlpp::CommandQueue commandQueue = device.NewCommandQueue();
    assert(commandQueue);

    const uint32_t dataCount = 1000;
    const uint16_t statistics = 5000;

    mtlpp::Buffer inBuffer = device.NewBuffer(sizeof(uint32_t) * dataCount, mtlpp::ResourceOptions::StorageModeManaged);
    assert(inBuffer);

    mtlpp::Buffer outBuffer = device.NewBuffer(sizeof(int) * dataCount, mtlpp::ResourceOptions::StorageModeManaged);
    assert(outBuffer);

    std::ofstream dataFile;
    dataFile.open("randoms.dat", std::ios::out /*| std::ios::app*/);
    if(!dataFile.is_open()){
      std::cerr<<"File to store data not opened"<<std::endl;
      return -1;
    }

    uint32_t i = 0;
    bool ended = false;
    bool pushing = false;
    auto tPrint = std::thread(printStatus, std::ref(i), std::ref(ended));
    tPrint.detach();

    std::queue<int*> buffer;
    auto tWrite = std::thread(streamToFile<int *>, std::ref(buffer), std::ref(dataCount), std::ref(dataFile), std::ref(ended), std::ref(pushing));

    auto gpu_start = std::chrono::steady_clock::now();
    for (; i < statistics; i++)
    {
      // update input data
      {
        float *inData = static_cast<float *>(inBuffer.GetContents());
        for (uint32_t j = 0; j < dataCount; j++)
          inData[j] = i * dataCount + j;
        inBuffer.DidModify(ns::Range(0, sizeof(float) * dataCount));
      }

      mtlpp::CommandBuffer commandBuffer = commandQueue.CommandBuffer();
      assert(commandBuffer);

      mtlpp::ComputeCommandEncoder commandEncoder = commandBuffer.ComputeCommandEncoder();
      commandEncoder.SetBuffer(inBuffer, 0, 0);
      commandEncoder.SetBuffer(outBuffer, 0, 1);
      commandEncoder.SetComputePipelineState(computePipelineState);
      commandEncoder.DispatchThreadgroups(
          mtlpp::Size(1, 1, 1),
          mtlpp::Size(dataCount, 1, 1));
      commandEncoder.EndEncoding();

      mtlpp::BlitCommandEncoder blitCommandEncoder = commandBuffer.BlitCommandEncoder();
      blitCommandEncoder.Synchronize(outBuffer);
      blitCommandEncoder.EndEncoding();

      commandBuffer.Commit();
      commandBuffer.WaitUntilCompleted();

    
        // read the data
        {
            //float* inData = static_cast<float*>(inBuffer.GetContents());
            auto outData = static_cast<int*>(outBuffer.GetContents());
            // for (uint32_t j=0; j<dataCount; j++){
          	// 	/*if (j%(dataCount/10)==0){
            //               	printf("sqr(%g) = %g\n", inData[j], outData[j]);
          	// 	}*/
            //   dataFile<<outData[j]<<std::endl;
            // }
            pushing = true;
            auto newplace = new int[dataCount];
            std::memcpy(newplace, std::move(outData), dataCount*sizeof(int));
            buffer.push(newplace);
            pushing = false;
      }
    }

    ended = true;

  if (tWrite.joinable()){
    std::cout<<std::endl<<"Waiting all threads to finish"<<std::endl;
    tWrite.join();
  };

  auto gpu_end = std::chrono::steady_clock::now();

  dataFile.close();

  std::cout << "Time difference (GPU) = " << std::chrono::duration_cast<std::chrono::milliseconds>(gpu_end - gpu_start).count() << "[ms]" << std::endl;
  return 0;
}
