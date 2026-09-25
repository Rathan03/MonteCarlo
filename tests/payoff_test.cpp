#include <gtest/gtest.h>
#include "monte_carlo/payoff.h"

#include <stdexcept>

using namespace MonteCarlo;

TEST(Payoff_Tests, Call_Payoff)
{
    Payoff payoff_ITM(120);
    double ITM = payoff_ITM.calculate_payoff(OptionType::Call, 100);

    Payoff payoff_OTM(80);
    double OTM = payoff_OTM.calculate_payoff(OptionType::Call, 100);

    Payoff payoff_ATM(100);
    double ATM = payoff_ATM.calculate_payoff(OptionType::Call, 100);

    EXPECT_DOUBLE_EQ(ITM, 20);
    EXPECT_DOUBLE_EQ(OTM,0);
    EXPECT_DOUBLE_EQ(ATM,0);
}

TEST(Payoff_Tests, Put_Payoff)
{
    Payoff payoff_ITM(80);
    double ITM = payoff_ITM.calculate_payoff(OptionType::Put, 100);

    Payoff payoff_OTM(120);
    double OTM = payoff_OTM.calculate_payoff(OptionType::Put, 100);

    Payoff payoff_ATM(100);
    double ATM = payoff_ATM.calculate_payoff(OptionType::Put, 100);

    EXPECT_DOUBLE_EQ(ITM, 20);
    EXPECT_DOUBLE_EQ(OTM,0);
    EXPECT_DOUBLE_EQ(ATM,0);
}

TEST(Payoff_Tests, European_From_Path)
{
    Payoff payoff({100,105,98,110,125});
    double price = payoff.calculate_payoff(OptionType::Call, 100);
    EXPECT_DOUBLE_EQ(price, 25);
}

TEST(Payoff_Tests, Asian_Call)
{
    Payoff payoff({100,110,90,120,80});
    double price = payoff.calculate_payoff(OptionType::AsianCall, 95);
    EXPECT_DOUBLE_EQ(price, 5);
}

TEST(Payoff_Tests, Asian_Put)
{
    Payoff payoff({100,110,90,120,80});
    double price = payoff.calculate_payoff(OptionType::AsianPut, 105);
    EXPECT_DOUBLE_EQ(price, 5);
}

TEST(Payoff_Tests, Throw_Errors)
{
    EXPECT_THROW(Payoff{-100}, std::invalid_argument);
    EXPECT_THROW(Payoff({}), std::invalid_argument);

    auto lambda_1 = []() {
        return Payoff({100,105,0,110});
    };

    auto lambda_2 = []() {
        return Payoff({100,105,-5,110});
    };

    auto lambda_3 = []() {
        return Payoff(100).calculate_payoff(OptionType::AsianCall, 95);
    };
    

    EXPECT_THROW(lambda_1(), std::invalid_argument);
    EXPECT_THROW(lambda_2(), std::invalid_argument);
    EXPECT_THROW(lambda_3(), std::domain_error);
}