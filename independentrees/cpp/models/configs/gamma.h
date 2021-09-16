#ifndef CONFIG_H
#define CONFIG_H

#include <random>
#include <pcg_random.hpp>
#include "generator.h"
#include "models.h"

typedef long double TData;

typedef std::gamma_distribution<TData> distribution;
typedef Generator<distribution, pcg32_fast, TData> rngenerator;
typedef GammaModel<rngenerator, distribution, TData> model;

#endif