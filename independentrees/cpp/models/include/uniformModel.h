#ifndef UNIFORMMODEL_H
#define UNIFORMMODEL_H

#include <cmath>
#include "pcg_random.hpp"

#include "model.h"

template <class TGenerator, class TDistribution, typename T>
class UniformModel : public Model<TGenerator,T>
{
public:
    static T draw(TGenerator *generator, const T &m)
    {
        auto beta = 1;
        auto a = m - beta * std::abs(m) - 1;
        auto b = m + beta * std::abs(m) + 1;

        auto dist = TDistribution(a, b);

        return generator->get_next(dist);
    };
};

#endif