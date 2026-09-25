#pragma once
#include "monte_carlo/random_generator.h"
#include <vector>
#include <utility>

namespace MonteCarlo
{
    class GBM
    {
        public:
            static double simulate(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator);
            static std::vector<double> simulate_path(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator);
            static std::pair<double,double> simulate_antithetic(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator);
            static std::pair<std::vector<double>,std::vector<double>> simulate_path_antithetic(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator);
    };
}