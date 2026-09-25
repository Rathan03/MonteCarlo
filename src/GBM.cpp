#include "monte_carlo/GBM.h"

#include <stdexcept>
#include <cmath>

namespace MonteCarlo
{
    double GBM::simulate(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator)
    {
        if (initial_price <= 0)
        {
            throw std::invalid_argument("Initial price must be positive.");
        }        
        if (years <= 0)
        {
            throw std::invalid_argument("Simulation years must be positive.");
        }
        if (number_of_timesteps <= 0)
        {
            throw std::invalid_argument("Number of timesteps must be positive.");
        }
        if (vol <= 0)
        {
            throw std::invalid_argument("Volatility must be positive.");
        }

        double dt = years/number_of_timesteps;
        double current_S{initial_price};

        for (int i{}; i< number_of_timesteps;i++)
        {
            current_S = current_S * std::exp((risk_free_rate-0.5 * vol * vol)* dt + vol * std::sqrt(dt) * generator.get_normal());
        }
        return current_S;
    }

}