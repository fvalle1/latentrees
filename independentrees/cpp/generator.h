#include <random>

template <class TDistribution, class TDevice, typename T>
class Generator{
    public:

    Generator():random_device(frd()){};
    Generator(const Generator& other):random_device(other.random_device){};

    T get_next(){return TDistribution(frd);};
    T get_next(TDistribution &dist) { return dist(random_device); };

private:
    std::random_device frd;
    TDevice random_device;
    TDistribution fDistribution;
};