#pragma once

#include <vector>

namespace MonteCarlo
{
    enum class OptionType
    {
        Call,
        Put,
        AsianCall,
        AsianPut
    };

    class Payoff
    {
        public:
            Payoff(double terminal_price);
            Payoff(std::vector<double>&& price_path);

            double calculate_payoff(OptionType option_type, double strike) const;
        
        private:
            double terminal_price;
            std::vector<double> price_path;

            double call_payoff(double price, double strike) const;
            double put_payoff(double price, double strike) const;
    };
}