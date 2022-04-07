#ifndef NBINMODEL_H
#define NBINMODEL_H

#include <cmath>
#include "pcg_random.hpp"

#include "model.h"

template <class TGenerator, class TDistribution, typename T>
class NbinModel : public Model<TGenerator,T>
{
public:
    static T draw(TGenerator *generator, const T &m)
    {
        auto mean = m + 2;
        auto n = mean * mean / (mean * mean - mean); //mu *mu / (sigma2 - mu)
        auto p = 1. / (mean);                        //mu/sigma2
        auto dist = TDistribution(n,p);

        return generator->get_next(dist);
    };
};

#endif