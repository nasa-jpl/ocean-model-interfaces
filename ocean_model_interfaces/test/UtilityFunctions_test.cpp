#include "ocean_model_interfaces/util/UtilityFunctions.h"
#include "ocean_model_interfaces/util/Point.h"

#include <gtest/gtest.h>

using namespace ocean_model_interfaces;

TEST(UtilityFunctionsTest, CalculateBarycentricCoordinates)
{
    auto b1 = calculateBarycentricCoordinates(Point(0,0,0),
                                              Point(1,0,0),
                                              Point(0,1,0),
                                              Point(0,0,0));
    EXPECT_DOUBLE_EQ(std::get<0>(b1), 1.0);
    EXPECT_DOUBLE_EQ(std::get<1>(b1), 0.0);
    EXPECT_DOUBLE_EQ(std::get<2>(b1), 0.0);

    auto b2 = calculateBarycentricCoordinates(Point(0,0,0),
                                              Point(1,0,0),
                                              Point(0,1,0),
                                              Point(1,0,0));
    EXPECT_DOUBLE_EQ(std::get<0>(b2), 0.0);
    EXPECT_DOUBLE_EQ(std::get<1>(b2), 1.0);
    EXPECT_DOUBLE_EQ(std::get<2>(b2), 0.0);

    auto b3 = calculateBarycentricCoordinates(Point(0,0,0),
                                              Point(1,0,0),
                                              Point(0,1,0),
                                              Point(0,1,0));
    EXPECT_DOUBLE_EQ(std::get<0>(b3), 0.0);
    EXPECT_DOUBLE_EQ(std::get<1>(b3), 0.0);
    EXPECT_DOUBLE_EQ(std::get<2>(b3), 1.0);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}