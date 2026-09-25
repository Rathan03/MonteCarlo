#pragma once
#include "monte_carlo/random_generator.h"
#include "monte_carlo/payoff.h"

namespace MonteCarlo
{
    class MonteCarloPricer
    {
        public:
            MonteCarloPricer(double initial_price, double years, double vol, double risk_free_rate, unsigned int seed,  double strike, OptionType option_type, int timesteps_per_year = 100, int number_of_paths = 1000);
            struct MonteCarloResult
            {
                double price;
                double standard_error;
            };
            MonteCarloResult price();
            MonteCarloResult price_antithetic();

        private:
            int timesteps_per_year;
            int number_of_paths;
            RandomGenerator generator;

            double initial_price;
            double years;
            double vol;
            double risk_free_rate;
            double strike;
            OptionType option_type;
    };
}
