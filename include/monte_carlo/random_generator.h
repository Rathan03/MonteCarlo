#pragma once
#include <random>

namespace MonteCarlo
{
    class RandomGenerator
    {
        public:
            RandomGenerator(unsigned int seed);
            double get_normal();
            double get_uniform();

        private:
            std::mt19937 engine;
            std::uniform_real_distribution<double> uniform;
            std::normal_distribution<double> normal;
    };
}