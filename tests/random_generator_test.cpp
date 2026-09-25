#include <gtest/gtest.h>
#include "monte_carlo/random_generator.h"

using namespace MonteCarlo;

TEST(RandomGeneratorTests, Same_Seed_Normal)
{
    RandomGenerator gen_1{1234};
    RandomGenerator gen_2{1234};
    for (int i{}; i <20 ; i++)
    {
        EXPECT_EQ(gen_1.get_normal(),gen_2.get_normal());
    }
}

TEST(RandomGeneratorTests, Same_Seed_Uniform)
{
    RandomGenerator gen_1{1234};
    RandomGenerator gen_2{1234};
    for (int i{}; i <20 ; i++)
    {
        EXPECT_EQ(gen_1.get_uniform(),gen_2.get_uniform());
    }
}

TEST(RandomGeneratorTests, Normal_Sense_Check)
{
    RandomGenerator gen{1234};
    int samples = 400;
    double sum{};
    double squared_sum{};

    for (int i{}; i < samples; i++)
    {   
        double temp{gen.get_normal()};
        sum += temp;
        squared_sum += temp * temp;
    }
    double mean = sum/samples;
    EXPECT_NEAR(mean, 0 , 0.1);
    EXPECT_NEAR(squared_sum/samples -mean* mean, 1 , 0.1);
}

TEST(RandomGeneratorTests, Uniform_Sense_Check)
{
    RandomGenerator gen{1234};

    for (int i{}; i < 30; i++)
    {   
        double temp{gen.get_uniform()};
        EXPECT_TRUE(0<= temp && temp < 1);
    }
}