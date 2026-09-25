#include "monte_carlo/montecarlopricer.h"
#include "monte_carlo/payoff.h"
#include "monte_carlo/GBM.h"
#include "monte_carlo/statistics.h"

#include <stdexcept>
#include <cmath>
#include <utility>

namespace MonteCarlo
{
    MonteCarloPricer::MonteCarloPricer(double initial_price, double years, double vol, double risk_free_rate, unsigned int seed,  double strike, OptionType option_type, int timesteps_per_year, int number_of_paths):
        initial_price{initial_price},
        years{years},
        vol{vol},
        risk_free_rate{risk_free_rate},
        strike{strike},
        option_type{option_type},
        timesteps_per_year{timesteps_per_year},
        number_of_paths{number_of_paths},
        generator{RandomGenerator(seed)}
    {}

    MonteCarloPricer::MonteCarloResult MonteCarloPricer::price()
    {
        if (option_type != OptionType::Call && option_type != OptionType::Put && option_type != OptionType::AsianCall && option_type != OptionType::AsianPut)
        {
            throw std::invalid_argument("Option type is not supported.");
        }
        
        int timesteps = static_cast<int>(std::round(timesteps_per_year*years));
        std::vector<double> payoffs;
        payoffs.reserve(number_of_paths);
        MonteCarloResult results;
        if (option_type == OptionType::Call || option_type == OptionType::Put)
        {
            for (int i{}; i < number_of_paths; i++)
            {
                payoffs.push_back(Payoff(GBM::simulate(initial_price, years, timesteps, vol, risk_free_rate, generator)).calculate_payoff(option_type, strike));
            }
        } else 
        {
            for (int i{}; i < number_of_paths; i++)
            {
                payoffs.push_back(Payoff(GBM::simulate_path(initial_price, years, timesteps, vol, risk_free_rate, generator)).calculate_payoff(option_type, strike));
            }
        }
        double discount_factor = std::exp(- risk_free_rate * years);
        results.price = discount_factor * Statistics::mean(payoffs);
        results.standard_error = discount_factor * Statistics::std_err(payoffs);
        return results;
    }

    MonteCarloPricer::MonteCarloResult MonteCarloPricer::price_antithetic()
    {
        if (option_type != OptionType::Call && option_type != OptionType::Put && option_type != OptionType::AsianCall && option_type != OptionType::AsianPut)
        {
            throw std::invalid_argument("Option type is not supported.");
        }
        
        int timesteps = static_cast<int>(std::round(timesteps_per_year*years));
        std::vector<double> payoffs;
        payoffs.reserve(number_of_paths);
        MonteCarloResult results;
        if (option_type == OptionType::Call || option_type == OptionType::Put)
        {
            for (int i{}; i < number_of_paths; i++)
            {
                auto [pos, neg] = GBM::simulate_antithetic(initial_price, years, timesteps, vol, risk_free_rate, generator);
                payoffs.push_back((Payoff{pos}.calculate_payoff(option_type, strike) + Payoff{neg}.calculate_payoff(option_type, strike))/2.0);
            }
        } else 
        {
            for (int i{}; i < number_of_paths; i++)
            {   
                auto [pos, neg] = GBM::simulate_path_antithetic(initial_price, years, timesteps, vol, risk_free_rate, generator);
                payoffs.push_back((Payoff{std::move(pos)}.calculate_payoff(option_type, strike) + Payoff{std::move(neg)}.calculate_payoff(option_type, strike))/2.0);
            }
        }
        double discount_factor = std::exp(- risk_free_rate * years);
        results.price = discount_factor * Statistics::mean(payoffs);
        results.standard_error = discount_factor * Statistics::std_err(payoffs);
        return results;
    }
}