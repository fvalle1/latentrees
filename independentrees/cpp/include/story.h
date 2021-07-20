#include <ostream>
#include <thread>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "generator.h"
#include "monitorThread.h"

std::mutex file_mutex;
std::mutex mon_mutex;

template <typename T, typename TT, class TGenerator, class TModel>
void makeSingleStory(std::ofstream &file, const T& first, TT &monitor){
    auto generator = new TGenerator();
    auto previous = first;
    bool skip_store = false;
    for (int step = 0; step < 25; step++)
    {
        previous = TModel::draw(generator, previous);

        if (previous > std::numeric_limits<T>::max())
        {
            skip_store = true;
            break;
        }

        //if (previous==0){break;};
    }

    if (skip_store)
        return;

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

template <typename T, typename TT, class TGenerator, class TModel>
void makeStories(std::ofstream &file, const T first, TT &monitorVal)
{
    for (int story = 0; story < 10000; story++)
    {
        makeSingleStory<T, TT, TGenerator, TModel>(file, first, monitorVal);
    }
}

#ifndef _OPENMP
template <typename TData, class TGenerator, class TModel>
void run(uint8_t n_threads = 12)
{
    auto file = std::ofstream("data.csv", std::ios::out);

    auto threads = std::vector<std::thread>();
    threads.reserve(n_threads);
    unsigned int monitor_val = 0;
    const unsigned int first = 10;
    bool ended = false;

    MonitorThread(monitor_val, ended).detach();

    for (size_t i = 0; i < n_threads; i++)
    {
        threads.push_back(std::thread(makeStories<TData, unsigned int, TGenerator, TModel>, std::ref(file), first, std::ref(monitor_val)));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    ended = true;

    file.close();
}

#else
template <typename TData, class TGenerator, class TModel>
void run(uint8_t n_threads = 12)
{
    omp_set_num_threads(n_threads);
    auto file = std::ofstream("data.csv", std::ios::out);

    unsigned int monitorVal = 0;
    const unsigned int first = 10;
    bool ended = false;
    #pragma omp master
    {
        std::cout<<"using OpenMP"<<std::endl;
    }

    MonitorThread(monitorVal, ended).detach();

#pragma omp parallel for
    for (int story = 0; story < 10000 * n_threads; story++)
    {
        makeSingleStory<TData, unsigned int, TGenerator, TModel>(file, first, monitorVal);
    }

    ended = true;

    file.close();
}

#endif