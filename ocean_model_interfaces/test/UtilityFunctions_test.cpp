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


TEST(UtilityFunctionsTest, LatLonToXY)
{
    Point origin1(-120.125, 30.5, -100.0);
    Point pointXY1 = latLonToLocalXY(origin1, origin1);
    Point pointLatLon1 = localXYToLatLon(origin1, pointXY1);

    EXPECT_NEAR(pointXY1.x, 0, 0.000001);
    EXPECT_NEAR(pointXY1.y, 0, 0.000001);
    EXPECT_NEAR(pointLatLon1.x, origin1.x, 0.000001);
    EXPECT_NEAR(pointLatLon1.y, origin1.y, 0.000001);
    EXPECT_NEAR(pointLatLon1.z, origin1.z, 0.000001);

    Point point2(-120.126, 30.54, -101.0);
    Point pointXY2 = latLonToLocalXY(origin1, point2);
    Point pointLatLon2 = localXYToLatLon(origin1, pointXY2);

    EXPECT_NEAR(pointLatLon2.x, point2.x, 0.000001);
    EXPECT_NEAR(pointLatLon2.y, point2.y, 0.000001);
    EXPECT_NEAR(pointLatLon2.z, point2.z, 0.000001);

    Point point3(600.123, -300.124, -101.0);
    Point pointLatLon3 = localXYToLatLon(origin1, point3);
    Point pointXY3 = latLonToLocalXY(origin1, pointLatLon3);

    EXPECT_NEAR(pointXY3.x, point3.x, 0.000001);
    EXPECT_NEAR(pointXY3.y, point3.y, 0.000001);
    EXPECT_NEAR(pointXY3.z, point3.z, 0.000001);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}