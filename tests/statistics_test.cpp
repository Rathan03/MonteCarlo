#include <gtest/gtest.h>
#include "monte_carlo/statistics.h"

#include <cmath>
#include <stdexcept>

using namespace MonteCarlo;

TEST(Stats_Test, Check_Example)
{
    double values[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_NEAR(Statistics::mean(values), 3.0, 1e-5);
    EXPECT_NEAR(Statistics::sample_variance(values), 2.5, 1e-5);
    EXPECT_NEAR(Statistics::std_err(values), std::sqrt(0.5), 1e-5);
}

TEST(Stats_Test, Mean_Throw)
{
    std::span<const double> values{};
    EXPECT_THROW(Statistics::mean(values), std::invalid_argument);
}

TEST(Stats_Test, Var_Throw)
{
    double values[1] = {1.0};
    EXPECT_THROW(Statistics::sample_variance(values), std::invalid_argument);
}

TEST(Stats_Test, Std_Err_Throw)
{
    double values[1] = {1.0};
    EXPECT_THROW(Statistics::std_err(values), std::invalid_argument);
}