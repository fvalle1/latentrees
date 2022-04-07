#ifndef GAMMAMODEL_H
#define GAMMAMODEL_H

#include <cmath>
#include "pcg_random.hpp"

#include "model.h"

template <class TGenerator, class TDistribution, typename T>
class GammaModel : public Model<TGenerator,T>
{
public:
    static T draw(TGenerator *generator, const T &m)
    {
        //mu=alpha*beta
        //std2 = alpha*beta*beta
        auto alpha = 1;
        auto beta = m+0.1;
        auto dist = TDistribution(alpha, beta);

        return generator->get_next(dist);
    };
};

#endif