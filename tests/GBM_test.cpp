#include <gtest/gtest.h>
#include "monte_carlo/GBM.h"
#include "monte_carlo/statistics.h"

#include <vector>

using namespace MonteCarlo;

TEST(GBM_Tests, Check_Throws)
{
    auto simulate_1 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate(-100,2,200,0.2,0.05,generator);
    };
    auto simulate_2 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate(100,-2,200,0.2,0.05,generator);
    };
    auto simulate_3 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate(100,2,-200,0.2,0.05,generator);
    };
    auto simulate_4 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate(100,2,200,-0.2,0.05,generator);
    };

    EXPECT_THROW(simulate_1(), std::invalid_argument);
    EXPECT_THROW(simulate_2(), std::invalid_argument);      
    EXPECT_THROW(simulate_3(), std::invalid_argument);
    EXPECT_THROW(simulate_4(), std::invalid_argument);
}

TEST(GBM_Tests, Check_Reproducible)
{
    RandomGenerator gen_1{1234};
    RandomGenerator gen_2{1234};

    double sim_1 = GBM::simulate(100,2,200,0.2,0.05,gen_1);
    double sim_2 = GBM::simulate(100,2,200,0.2,0.05,gen_2);

    EXPECT_EQ(sim_1,sim_2);
}

TEST(GBM_Tests, Expected_Mean)
{
    RandomGenerator gen{1234};
    std::vector<double> outputs;
    for (int i{}; i < 100000; i++)
    {
        outputs.push_back(GBM::simulate(100,1,100,0.2,0.05,gen));
    }

    EXPECT_NEAR(Statistics::mean(outputs), 100 *std::exp(0.05), 3 * Statistics::std_err(outputs));
}