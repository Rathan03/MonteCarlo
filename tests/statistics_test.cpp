#include <gtest/gtest.h>
#include "monte_carlo/statistics.h"

#include <cmath>

using namespace MonteCarlo;

TEST(Stats_Test, Check_Example)
{
    double values[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_NEAR(Statistics::mean(values), 3.0, 1e-5);
    EXPECT_NEAR(Statistics::sample_variance(values), 2.5, 1e-5);
    EXPECT_NEAR(Statistics::std_err(values), std::sqrt(0.5), 1e-5);
}