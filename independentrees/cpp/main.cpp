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

#include "uniform.h"
#include "gamma.h"
#include "nbinom.h"
#include "gaus.h"

int main()
{

    run<TData, rngenerator, model>();

    return 0;
}
