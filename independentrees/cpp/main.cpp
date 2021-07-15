#include <fstream>
#include <random>
#include <vector>
#include <thread>
#include <mutex>
#include <pcg_random.hpp>

#include "generator.h"
#include "models.h"

std::mutex file_mutex;
std::mutex mon_mutex;

using std::endl;
using std::cout;

typedef long TData;

typedef std::uniform_int_distribution<TData> distribution;
typedef Generator<distribution, pcg32_fast, TData> currentGenerator;
typedef UniformModel<currentGenerator, distribution, TData> model;

// typedef std::normal_distribution<TData> distribution;
// typedef Generator<distribution, pcg32_fast, TData> currentGenerator;
// typedef GausModel<currentGenerator, distribution, TData> model;

// typedef std::negative_binomial_distribution<TData> distribution;
// typedef Generator<distribution, pcg32_fast, TData> currentGenerator;
// typedef NbinModel<currentGenerator, distribution, TData> model;

template<typename T>
void monitor(const T& mon, const bool& ended){
    cout<<"starting monitor"<<endl;
    while(!ended){
        cout<<"\r"<<mon;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));        
    }
    cout <<endl;
    cout << "ending monitoring" << endl;
}

template <typename T, typename TT>
void make_story(std::ofstream &file, const T first, TT &monitor)
{
    for (int story = 0; story < 10000; story++)
    {
        currentGenerator* generator = new currentGenerator();
        auto previous = first;
        bool skip_store = false;
        for (int step = 0; step <25; step++)
        {
            previous = model::draw(generator, previous);

            if (previous > std::numeric_limits<T>::max())
            {
                skip_store=true;
                break;
            }

            //if (previous==0){break;};
        }

        if (skip_store) break;

        {
            std::lock_guard<std::mutex> lock(file_mutex);
            file << previous << std::endl;
        }
        delete generator;

        {
            std::lock_guard<std::mutex> lock(mon_mutex);
            monitor++;
        }
    }
}

void run(){
    auto fFile = std::ofstream("data.csv", std::ios::out);

    auto threads = std::vector<std::thread>();

    int monitor_val = 0;
    bool ended = false;
    uint8_t n_threads = 12;
    std::thread(monitor<int>, std::ref(monitor_val), std::ref(ended)).detach();

    for (size_t i = 0; i < n_threads; i++)
    {
        threads.push_back(std::thread(make_story<TData, int>, std::ref(fFile), 10, std::ref(monitor_val)));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    ended = true;

    fFile.close();
}

int main()
{

    run();

    return 0;
}