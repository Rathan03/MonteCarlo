#include <gtest/gtest.h>
#include "monte_carlo/montecarlopricer.h"

using namespace MonteCarlo;

TEST(Pricing_Test, Call_Price)
{
    MonteCarloPricer model(100, 1, 0.2, 0.05, 1234,  100, OptionType::Call);
    auto results = model.price();
    EXPECT_NEAR(results.price, 10.4506, 3*results.standard_error);
    EXPECT_TRUE(results.standard_error > 0);
}

TEST(Pricing_Test, Put_Price)
{
    MonteCarloPricer model(100, 1, 0.2, 0.05, 1234,  100, OptionType::Put);
    auto results = model.price();
    EXPECT_NEAR(results.price, 5.5735, 3*results.standard_error);
    EXPECT_TRUE(results.standard_error > 0);
}

TEST(Pricing_Test, Reproducibility_Test)
{
    MonteCarloPricer model_1(100, 1, 0.2, 0.05, 1234,  100, OptionType::Call);
    auto result_1 = model_1.price();

    MonteCarloPricer model_2(100, 1, 0.2, 0.05, 1234,  100, OptionType::Call);
    auto result_2 = model_2.price();
    
    EXPECT_EQ(result_1.price, result_2.price);
    EXPECT_EQ(result_1.standard_error, result_2.standard_error);
}

TEST(Pricing_Test, Invalid_Option)
{
    auto lambda = []() {
        return MonteCarloPricer(100, 1, 0.2, 0.05, 1234,  100, static_cast<OptionType>(5)).price();
    };

    EXPECT_THROW(lambda(), std::invalid_argument);
}

TEST(Pricing_Test, Antithetic_Call_Price)
{
    MonteCarloPricer model(
        100, 1, 0.2, 0.05, 1234, 100, OptionType::Call
    );

    auto results = model.price_antithetic();

    EXPECT_NEAR(results.price, 10.4506, 3 * results.standard_error);
    EXPECT_TRUE(results.standard_error > 0);
}

TEST(Pricing_Test, Antithetic_Put_Price)
{
    MonteCarloPricer model(
        100, 1, 0.2, 0.05, 1234, 100, OptionType::Put
    );

    auto results = model.price_antithetic();

    EXPECT_NEAR(results.price, 5.5735, 3 * results.standard_error);
    EXPECT_TRUE(results.standard_error > 0);
}

TEST(Pricing_Test, Antithetic_Reproducibility)
{
    MonteCarloPricer model_1(
        100, 1, 0.2, 0.05, 1234, 100, OptionType::Call
    );

    MonteCarloPricer model_2(
        100, 1, 0.2, 0.05, 1234, 100, OptionType::Call
    );

    auto result_1 = model_1.price_antithetic();
    auto result_2 = model_2.price_antithetic();

    EXPECT_EQ(result_1.price, result_2.price);
    EXPECT_EQ(result_1.standard_error, result_2.standard_error);
}

TEST(Pricing_Test, Antithetic_Asian_Call)
{
    MonteCarloPricer model(
        100, 1, 0.2, 0.05, 1234, 100, OptionType::AsianCall
    );

    auto results = model.price_antithetic();

    EXPECT_GT(results.price, 0);
    EXPECT_GT(results.standard_error, 0);
}

TEST(Pricing_Test, Antithetic_Asian_Put)
{
    MonteCarloPricer model(
        100, 1, 0.2, 0.05, 1234, 100, OptionType::AsianPut
    );

    auto results = model.price_antithetic();

    EXPECT_GT(results.price, 0);
    EXPECT_GT(results.standard_error, 0);
}