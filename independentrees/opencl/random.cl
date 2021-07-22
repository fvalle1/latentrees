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

float absolute(float x){
    if (x>0){
        return x;
    }else{
        return -x;
    }
}

__kernel void storyMersenne(__global float *vIn, __global float *vOut, const unsigned int count  ){
    int id = get_global_id(0); 

    int rng_state = wang_hash(vIn[id]);
    float temp = 10.0;

    if(id < count){

    for (int l = 0; l < 50; l++){
        float delta = 1.0 + 1.73 * absolute(temp);
        rng_state = rand_xorshift(rng_state);
        float r = 0.5 + rng_state * (1.0 / 4294967296.0);
        temp = round(2*r*delta+temp-delta);
        if(temp > 1000000000000000){
        temp = 1000000000000000;
        }
    }

    vOut[id] = temp;
    }

}