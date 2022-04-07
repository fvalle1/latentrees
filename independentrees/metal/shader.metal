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