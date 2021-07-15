#include <gtest/gtest.h>

#include "models.h"
#include "generator.h"
#include "pcg_random.hpp"

// Demonstrate some basic assertions.
TEST(defTest, BasicAssertions)
{
    typedef int TData;
    typedef std::negative_binomial_distribution<TData> distribution;
    typedef Generator<distribution, pcg32_fast, TData> currentGenerator;
    typedef NbinModel<currentGenerator, distribution, TData> model;

    auto generator = currentGenerator();
}
