#include <gtest/gtest.h>

#include "models.h"
#include "generator.h"
#include "monitorThread.h"
#include "pcg_random.hpp"

// Demonstrate some basic assertions.
TEST(defTest, BasicAssertions)
{
    typedef int TData;
    typedef std::negative_binomial_distribution<TData> distribution;
    typedef Generator<distribution, pcg32_fast, TData> rngenerator;
    typedef NbinModel<rngenerator, distribution, TData> model;

    auto generator = rngenerator();
}

TEST(defTestThread, BasicAssertions)
{
    int a = 0;
    bool end = false;
    auto monitor = MonitorThread<int>(a, end);
    monitor.detach();

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
