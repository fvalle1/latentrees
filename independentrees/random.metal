#ifndef RANDOM_H
#define RANDOM_H

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

#endif