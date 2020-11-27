#include "mtlpp.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>

void printStatus(uint16_t &iStat, bool &ended)
{
  while(!ended){
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout<<"\r"<<iStat;
}
  std::cout<<std::endl;
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
            const device float *vIn [[ buffer(0) ]],
            device float *vOut [[ buffer(1) ]],
            uint id[[ thread_position_in_grid ]]){
          
          mt19937 mers;

          mers.srand(vIn[id]);
         
          float temp = 10.0;

          for (uint l = 0; l < 50; l++){
              float delta = 1.0 + sqrt(3.0)*abs(temp);
              float r = mers.rand();
              temp = round(2*r*delta-abs(temp));
              if(temp > 1000000000000000){
              temp = 1000000000000000;
              }
          }

          vOut[id] = temp;
                  
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
    bool ended = false;

    mtlpp::Buffer inBuffer = device.NewBuffer(sizeof(float) * dataCount, mtlpp::ResourceOptions::StorageModeManaged);
    assert(inBuffer);

    mtlpp::Buffer outBuffer = device.NewBuffer(sizeof(float) * dataCount, mtlpp::ResourceOptions::StorageModeManaged);
    assert(outBuffer);

    std::ofstream dataFile;
    dataFile.open("randoms.dat", std::ios::out /*| std::ios::app*/);
    if(!dataFile.is_open()){
      std::cerr<<"File to store data not opened"<<std::endl;
      return -1;
    }

    uint16_t i = 0;
    auto statusThread = std::thread(printStatus, std::ref(i), std::ref(ended));
    statusThread.detach();

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
        float *inData = static_cast<float *>(inBuffer.GetContents());
        float *outData = static_cast<float *>(outBuffer.GetContents());
        for (uint32_t j = 0; j < dataCount; j++)
        {
          /*if (j%(dataCount/10)==0){
                          	printf("sqr(%g) = %g\n", inData[j], outData[j]);
          		}*/
          dataFile << outData[j] << std::endl;
        }
      }
    }

    auto gpu_end = std::chrono::steady_clock::now();
    ended = true;
    dataFile.close();



    std::cout << "Time difference (GPU) = " << std::chrono::duration_cast<std::chrono::milliseconds> (gpu_end - gpu_start).count() << "[ms]" << std::endl;



    return 0;
}
