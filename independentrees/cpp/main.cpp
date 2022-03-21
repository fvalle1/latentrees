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

//only the first of these will be compiled by preprocessor due to the #ifdef
//#include "gaus.h"
// #include "randomwalk.h"
#include "uniform.h"
// #include "gamma.h"
// #include "nbinom.h"

#include <sstream>

int main()
{
    for(int i = 0; i < 1; i++){
        std::stringstream ss;
        ss<<"data_"<<i<<".csv";
        run<TData, rngenerator, model>(120000, 12, ss.str().c_str());
    }
    return 0;
}
