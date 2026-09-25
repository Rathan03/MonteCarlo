#pragma once
#include <span>

namespace MonteCarlo
{
    class Statistics
    {
        public:
            static double mean(std::span<const double> span);
            static double sample_variance(std::span<const double> span);
            static double std_err(std::span<const double> span);
    };
}