#ifndef MODEL_H
#define MODEL_H

template <class TGenerator, typename T>
class Model
{
public:
    static T draw(TGenerator *generator, const T &m);
};

#endif