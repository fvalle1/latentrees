#include "mtlpp.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cmath>

int main()
{
using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    mtlpp::Device device = mtlpp::Device::CreateSystemDefaultDevice();
    assert(device);

    const char shadersSrc[] = R"""(
        #include <metal_stdlib>

        using namespace metal;

        uint wang_hash(uint seed)
        {
            seed = (seed ^ 61) ^ (seed >> 16);
            seed *= 9;
            seed = seed ^ (seed >> 4);
            seed *= 0x27d4eb2d;
            seed = seed ^ (seed >> 15);
            return seed;
        }

        uint rand_xorshift(uint rng_state)
        {
            // Xorshift algorithm from George Marsaglia's paper
            rng_state ^= (rng_state << 13);
            rng_state ^= (rng_state >> 17);
            rng_state ^= (rng_state << 5);
            return rng_state;
        }


        kernel void storyKernel(
            const device float *vIn [[ buffer(0) ]],
            device float *vOut [[ buffer(1) ]],
            uint id[[ thread_position_in_grid ]])
        {
          uint rng_state = wang_hash(vIn[id]);
          float temp = 10.0;

          for (uint l = 0; l < 50; l++){
              float delta = 1.0 + sqrt(3.0)*abs(temp);
              rng_state = rand_xorshift(rng_state);
              float r = rng_state * (1.0 / 4294967296.0);
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
    mtlpp::Function sqrFunc = library.NewFunction("storyKernel");
    assert(sqrFunc);

    mtlpp::ComputePipelineState computePipelineState = device.NewComputePipelineState(sqrFunc, nullptr);
    assert(computePipelineState);

    mtlpp::CommandQueue commandQueue = device.NewCommandQueue();
    assert(commandQueue);

    const uint32_t dataCount = 5000;
    const uint16_t statistics = 1000;

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
    auto gpu_start = std::chrono::steady_clock::now();
      for (auto i=0;i<statistics;i++){
        // update input data
        {
            float* inData = static_cast<float*>(inBuffer.GetContents());
            for (uint32_t j=0; j<dataCount; j++)
                inData[j] = i*dataCount+j;
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
            float* inData = static_cast<float*>(inBuffer.GetContents());
            float* outData = static_cast<float*>(outBuffer.GetContents());
            for (uint32_t j=0; j<dataCount; j++){
          		/*if (j%(dataCount/10)==0){
                          	printf("sqr(%g) = %g\n", inData[j], outData[j]);
          		}*/
              dataFile<<outData[j]<<std::endl;
            }
      }
    }

    auto gpu_end = std::chrono::steady_clock::now();
    dataFile.close();



    std::cout << "Time difference (GPU) = " << std::chrono::duration_cast<std::chrono::milliseconds> (gpu_end - gpu_start).count() << "[ms]" << std::endl;



    return 0;
}
