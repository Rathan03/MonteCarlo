#include "monte_carlo/random_generator.h"

namespace MonteCarlo
{
    RandomGenerator::RandomGenerator(unsigned int seed):
        engine(seed),
        normal(0.0,1.0),
        uniform(0.0,1.0)
    {}

    double RandomGenerator::get_normal()
    {
        return normal(engine);
    }

    double RandomGenerator::get_uniform()
    {
        return uniform(engine);  
    }
}