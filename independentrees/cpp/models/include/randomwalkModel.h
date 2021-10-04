#ifndef RANDOMWALKMODEL_H
#define RANDOMWALKMODEL_H

#include <cmath>
#include "pcg_random.hpp"

#include "model.h"

template <class TGenerator, class TDistribution, typename T>
class RandomWalkModel : public Model<TGenerator,T>
{
public:
    static T draw(TGenerator *generator, const T &m)
    {
        auto a = 1;
        auto b = 2 * std::abs(m);

        auto dist = TDistribution(a, b);

        return generator->get_next(dist);
    };
};

#endif