#include <fstream>
#include <random>
#include <vector>
#include <thread>
#include <mutex>
#include <pcg_random.hpp>

#include "generator.h"
#include "monitorThread.h"
#include "models.h"
#include "story.h"


using std::endl;
using std::cout;

typedef long double TData;
typedef pcg64 pcg_t;

// typedef std::uniform_int_distribution<TData> distribution;
// typedef Generator<distribution, pcg_t, TData> rngenerator;
// typedef UniformModel<rngenerator, distribution, TData> model;

// typedef std::normal_distribution<TData> distribution;
// typedef Generator<distribution, pcg_t, TData> rngenerator;
// typedef GausModel<rngenerator, distribution, TData> model;

// typedef std::negative_binomial_distribution<TData> distribution;
// typedef Generator<distribution, pcg_t, TData> rngenerator;
// typedef NbinModel<rngenerator, distribution, TData> model;

typedef std::gamma_distribution<TData> distribution;
typedef Generator<distribution, pcg_t, TData> rngenerator;
typedef GammaModel<rngenerator, distribution, TData> model;


int main()
{

    run<TData, rngenerator, model>();

    return 0;
}