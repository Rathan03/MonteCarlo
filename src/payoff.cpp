#include "monte_carlo/payoff.h"
#include "monte_carlo/statistics.h"

#include <stdexcept>
#include <cmath>

namespace MonteCarlo
{
    Payoff::Payoff(double terminal_price):
        terminal_price{terminal_price}
    {
        if (terminal_price <= 0)
        {
            throw std::invalid_argument("Terminal price must be positive.");
        }
    }

    Payoff::Payoff(std::vector<double>&& price_path):
        price_path{price_path}
    {
        if (price_path.empty())
        {
            throw std::invalid_argument("Price path cannot be empty.");
        }
        for (double price : price_path)
        {
            if(price <=0)
            {
                throw std::invalid_argument("Prices in price path must be strictly positive.");
            }
        }
    }

    double Payoff::call_payoff(double price, double strike) const
    {
        return std::max(price-strike, 0.0);
    }

    double Payoff::put_payoff(double price, double strike) const
    {
        return std::max(strike-price, 0.0);
    }

    double Payoff::calculate_payoff(OptionType option_type, double strike) const
    {
        if((option_type == OptionType::AsianCall || option_type == OptionType::AsianPut) && price_path.empty())
        {
            throw std::domain_error("Need payoff object with price path information to compute payoff for Asian options");
        }

        switch (option_type)
        {
        case OptionType::AsianCall:
        {
            return call_payoff(Statistics::mean(price_path), strike);
        }
        case OptionType::AsianPut:
        {
            return put_payoff(Statistics::mean(price_path), strike);
        }
        case OptionType::Call:
        {
            double price = price_path.size() == 0 ? terminal_price : price_path.back();
            return call_payoff(price, strike);
        }
        case OptionType::Put:
        {
            double price = price_path.size() == 0 ? terminal_price : price_path.back();
            return put_payoff(price, strike);
        }
        default:
            throw std::domain_error("Option type does not exist.");
        }

    }
}