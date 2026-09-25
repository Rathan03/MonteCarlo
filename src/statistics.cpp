#include "monte_carlo/statistics.h"

#include <span>
#include <cmath>
#include <stdexcept>

namespace MonteCarlo
{
    double Statistics::mean(std::span<const double> span)
    {   
        std::size_t size{span.size()};
        if (size == 0)
        {
            throw std::invalid_argument("Number of elements must be positive.");
        }

        double sum{};
        for (double value : span)
        {
            sum += value;
        }
        return sum/ size;
    }

    double Statistics::sample_variance(std::span<const double> span)
    {
        std::size_t size{span.size()};
        if (size <= 1)
        {
            throw std::invalid_argument("Must have 2 or more elements to calculate variance.");
        }

        double numerator{};
        double mean{Statistics::mean(span)};

        for (double value : span)
        {
            numerator += (value-mean) * (value - mean);
        }

        return numerator / (size - 1);
    }

    double Statistics::std_err(std::span<const double> span)
    {
        return std::sqrt(Statistics::sample_variance(span)/span.size());
    }
}