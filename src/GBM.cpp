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

    std::vector<double> GBM::simulate_path(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator)
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
        std::vector<double> output;
        output.reserve(number_of_timesteps + 1);
        output.push_back(initial_price);

        for (int i{}; i< number_of_timesteps;i++)
        {
            output.push_back(output.back() * std::exp((risk_free_rate-0.5 * vol * vol)* dt + vol * std::sqrt(dt) * generator.get_normal()));
        }
        return output;
    }

    std::pair<double,double> GBM::simulate_antithetic(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator)
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
        double current_pos{initial_price};
        double current_neg{initial_price};

        for (int i{}; i< number_of_timesteps;i++)
        {
            double sample{generator.get_normal()};
            current_pos = current_pos * std::exp((risk_free_rate-0.5 * vol * vol)* dt + vol * std::sqrt(dt) * sample);
            current_neg = current_neg * std::exp((risk_free_rate-0.5 * vol * vol)* dt - vol * std::sqrt(dt) * sample);
        }
        return std::make_pair(current_pos, current_neg);
    }

    std::pair<std::vector<double>,std::vector<double>> simulate_path_antithetic(double initial_price, double years, int number_of_timesteps, double vol, double risk_free_rate, RandomGenerator& generator)
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
        std::vector<double> output_pos;
        output_pos.reserve(number_of_timesteps + 1);
        output_pos.push_back(initial_price);

        std::vector<double> output_neg;
        output_neg.reserve(number_of_timesteps + 1);
        output_neg.push_back(initial_price);

        for (int i{}; i< number_of_timesteps;i++)
        {
            double sample{generator.get_normal()};
            output_pos.push_back(output_pos.back() * std::exp((risk_free_rate-0.5 * vol * vol)* dt + vol * std::sqrt(dt) * sample));
            output_neg.push_back(output_neg.back() * std::exp((risk_free_rate-0.5 * vol * vol)* dt - vol * std::sqrt(dt) * sample));
        }
        return std::make_pair(output_pos, output_neg);
    }
}