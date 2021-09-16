#ifndef CONFIG_H
#define CONFIG_H

#include <random>
#include <pcg_random.hpp>
#include "generator.h"
#include "models.h"

typedef long long TData;

typedef std::negative_binomial_distribution<TData> distribution;
typedef Generator<distribution, pcg32_fast, TData> rngenerator;
typedef NbinModel<rngenerator, distribution, TData> model;

#endif