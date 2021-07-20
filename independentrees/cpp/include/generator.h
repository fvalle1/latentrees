#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>

template <class TDistribution, class TDevice, typename T>
class Generator{
    public:

    Generator():frandomDevice(frandomSeed()){};
    Generator(const Generator& other):frandomDevice(other.frandomDevice){};

    inline T get_next(){return TDistribution(frandomSeed);};
    inline T get_next(TDistribution &dist) { return dist(frandomDevice); };

private:
    std::random_device frandomSeed;
    TDevice frandomDevice;
    TDistribution fDistribution;
};

#endif