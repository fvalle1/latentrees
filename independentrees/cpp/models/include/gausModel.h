#ifndef GAUSMODEL_H
#define GAUSMODEL_H

#include <cmath>
#include "pcg_random.hpp"

#include "model.h"

template <class TGenerator, class TDistribution, typename T>
class GausModel : public Model<TGenerator,T>
{
public:
    static T draw(TGenerator *generator, const T &m)
    {
        auto dist = TDistribution(std::round(m), std::abs(std::round(m)));

        return generator->get_next(dist);
    };
};

#endif