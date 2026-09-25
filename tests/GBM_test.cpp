#include <gtest/gtest.h>
#include "monte_carlo/GBM.h"
#include "monte_carlo/statistics.h"

#include <vector>

using namespace MonteCarlo;

TEST(GBM_Tests, Check_Throws_Simulate)
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
    for (int i{}; i < 10000; i++)
    {
        outputs.push_back(GBM::simulate(100,1,100,0.2,0.05,gen));
    }

    EXPECT_NEAR(Statistics::mean(outputs), 100 *std::exp(0.05), 3 * Statistics::std_err(outputs));
}

TEST(GBM_Tests, Check_Throws_Simulate_Path)
{
    auto simulate_1 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate_path(-100,2,200,0.2,0.05,generator);
    };
    auto simulate_2 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate_path(100,-2,200,0.2,0.05,generator);
    };
    auto simulate_3 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate_path(100,2,-200,0.2,0.05,generator);
    };
    auto simulate_4 = []() {
        RandomGenerator generator(1234);
        return GBM::simulate_path(100,2,200,-0.2,0.05,generator);
    };

    EXPECT_THROW(simulate_1(), std::invalid_argument);
    EXPECT_THROW(simulate_2(), std::invalid_argument);      
    EXPECT_THROW(simulate_3(), std::invalid_argument);
    EXPECT_THROW(simulate_4(), std::invalid_argument);
}

TEST(GBM_Tests, Path_Size_Check)
{
    int number_of_timesteps = 100;
    RandomGenerator generator(1234);
    auto vec = GBM::simulate_path(100,2,number_of_timesteps,0.2,0.05,generator);
    EXPECT_EQ(vec.size(), number_of_timesteps+1);
}

TEST(GBM_Tests, First_Elem_Check)
{
    double init_price = 100;
    RandomGenerator generator(1234);
    auto vec = GBM::simulate_path(init_price,2,100,0.2,0.05,generator);
    EXPECT_EQ(vec[0], init_price);
}

TEST(GBM_Tests, Path_Reproducibility_Check)
{
    RandomGenerator generator_1(1234);
    auto vec_1 = GBM::simulate_path(100,2,100,0.2,0.05,generator_1);
    RandomGenerator generator_2(1234);
    auto vec_2 = GBM::simulate_path(100,2,100,0.2,0.05,generator_2);
    EXPECT_EQ(vec_1, vec_2);
}

TEST(GBM_Tests, Strictly_Positive)
{
    RandomGenerator generator_1(1234);
    auto vec_1 = GBM::simulate_path(1,40,100,3,0.05,generator_1);
    
    for (double price : vec_1)
    {
        EXPECT_TRUE(price >0);
    }
}

TEST(GBM_Tests, Strictly_Positive_Antithetic)
{
    RandomGenerator generator_1(1234);
    RandomGenerator generator_2(1234);

    auto result_1 = GBM::simulate_antithetic(100,2,100,0.2,0.05, generator_1);
    auto result_2 = GBM::simulate_antithetic(100,2,100,0.2,0.05, generator_2);

    EXPECT_EQ(result_1.first, result_2.first);
    EXPECT_EQ(result_1.second, result_2.second);
}

TEST(GBM_Test, Antithetic_Terminal_Price)
{
    RandomGenerator generator(1234);
    RandomGenerator expected_generator(1234);

    double initial_price = 100;
    double years = 1;
    int timesteps = 10;
    double vol = 0.2;
    double risk_free_rate = 0.05;

    auto result = GBM::simulate_antithetic(
        initial_price,
        years,
        timesteps,
        vol,
        risk_free_rate,
        generator
    );

    double dt = years / timesteps;

    double expected_positive = initial_price;
    double expected_negative = initial_price;

    for (int i{}; i < timesteps; i++)
    {
        double z = expected_generator.get_normal();

        expected_positive *= std::exp(
            (risk_free_rate - 0.5 * vol * vol) * dt
            + vol * std::sqrt(dt) * z
        );

        expected_negative *= std::exp(
            (risk_free_rate - 0.5 * vol * vol) * dt
            - vol * std::sqrt(dt) * z
        );
    }

    EXPECT_DOUBLE_EQ(result.first, expected_positive);
    EXPECT_DOUBLE_EQ(result.second, expected_negative);
}

TEST(GBM_Test, Antithetic_Path)
{
    RandomGenerator generator(1234);

    double initial_price = 100;
    int timesteps = 10;

    auto result = GBM::simulate_path_antithetic(
        initial_price,
        1,
        timesteps,
        0.2,
        0.05,
        generator
    );

    const auto& positive_path = result.first;
    const auto& negative_path = result.second;

    EXPECT_EQ(positive_path.size(), timesteps + 1);
    EXPECT_EQ(negative_path.size(), timesteps + 1);

    EXPECT_DOUBLE_EQ(positive_path.front(), initial_price);
    EXPECT_DOUBLE_EQ(negative_path.front(), initial_price);

    for (double price : positive_path)
    {
        EXPECT_GT(price, 0);
    }

    for (double price : negative_path)
    {
        EXPECT_GT(price, 0);
    }
}

TEST(GBM_Test, Antithetic_Path_Reproducibility)
{
    RandomGenerator generator_1(1234);
    RandomGenerator generator_2(1234);

    auto result_1 = GBM::simulate_path_antithetic(
        100, 1, 10, 0.2, 0.05, generator_1
    );

    auto result_2 = GBM::simulate_path_antithetic(
        100, 1, 10, 0.2, 0.05, generator_2
    );

    EXPECT_EQ(result_1.first, result_2.first);
    EXPECT_EQ(result_1.second, result_2.second);
}