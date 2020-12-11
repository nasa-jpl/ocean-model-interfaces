#include "ocean_models/general_models/OceanFrontModel.h"

#include "ocean_models/model_interface/ModelData.h"

#include <gtest/gtest.h>

using namespace ocean_models;

TEST(OceanFrontModelTest, Outside) {

    OceanFrontModel::Parameters parameters0Deg;
    parameters0Deg.frontOrientation = 0.0;
    parameters0Deg.frontWidth = 1000;
    parameters0Deg.frontX = 0;
    parameters0Deg.frontY = 0;
    parameters0Deg.depths = {0,5,10,20,30};
    parameters0Deg.side1Temps = {20,19,18,17,16};
    parameters0Deg.side2Temps = {20,18,16,14,12};
    parameters0Deg.side1Salts = {30,29,28,27,26};
    parameters0Deg.side2Salts = {30,28,26,24,22};
    parameters0Deg.currentU = 1;
    parameters0Deg.currentV = -1;
    parameters0Deg.dye = 2;

    OceanFrontModel model0Deg(parameters0Deg);

    OceanFrontModel::Parameters parameters90Deg;
    parameters90Deg.frontOrientation = -3.14159265359 / 2;
    parameters90Deg.frontWidth = 2000;
    parameters90Deg.frontX = 2000;
    parameters90Deg.frontY = -1000;
    parameters90Deg.depths = {0,5,10,20,30};
    parameters90Deg.side1Temps = {20,19,18,17,16};
    parameters90Deg.side2Temps = {20,18,16,14,12};
    parameters90Deg.side1Salts = {30,29,28,27,26};
    parameters90Deg.side2Salts = {30,28,26,24,22};

    OceanFrontModel model90Deg(parameters90Deg);

    ModelData deg0Data1 = model0Deg.getData(-10000, 1500, 20, 0);
    ModelData deg0Data2 = model0Deg.getData(10000, 1500, 22, 0);

    ModelData deg0Data3 = model0Deg.getData(-20000, -1500, 20, 0);
    ModelData deg0Data4 = model0Deg.getData(20000, -1500, 22, 0);

    EXPECT_FLOAT_EQ(14.0, deg0Data1.temp);
    EXPECT_FLOAT_EQ(13.6, deg0Data2.temp);
    EXPECT_FLOAT_EQ(17.0, deg0Data3.temp);
    EXPECT_FLOAT_EQ(16.8, deg0Data4.temp);

    EXPECT_FLOAT_EQ(24.0, deg0Data1.salt);
    EXPECT_FLOAT_EQ(23.6, deg0Data2.salt);
    EXPECT_FLOAT_EQ(27.0, deg0Data3.salt);
    EXPECT_FLOAT_EQ(26.8, deg0Data4.salt);

    EXPECT_FLOAT_EQ(1, deg0Data1.u);
    EXPECT_FLOAT_EQ(1, deg0Data2.u);
    EXPECT_FLOAT_EQ(1, deg0Data3.u);
    EXPECT_FLOAT_EQ(1, deg0Data4.u);

    EXPECT_FLOAT_EQ(-1, deg0Data1.v);
    EXPECT_FLOAT_EQ(-1, deg0Data2.v);
    EXPECT_FLOAT_EQ(-1, deg0Data3.v);
    EXPECT_FLOAT_EQ(-1, deg0Data4.v);

    EXPECT_FLOAT_EQ(2, deg0Data1.dye);
    EXPECT_FLOAT_EQ(2, deg0Data2.dye);
    EXPECT_FLOAT_EQ(2, deg0Data3.dye);
    EXPECT_FLOAT_EQ(2, deg0Data4.dye);



    ModelData deg90Data1 = model90Deg.getData(4500, -5000, 20, 0);
    ModelData deg90Data2 = model90Deg.getData(4500, -10000, 22, 0);

    ModelData deg90Data3 = model90Deg.getData(-500, 5000, 20, 0);
    ModelData deg90Data4 = model90Deg.getData(-500, 10000, 22, 0);

    EXPECT_FLOAT_EQ(14.0, deg90Data1.temp);
    EXPECT_FLOAT_EQ(13.6, deg90Data2.temp);
    EXPECT_FLOAT_EQ(17.0, deg90Data3.temp);
    EXPECT_FLOAT_EQ(16.8, deg90Data4.temp);

    EXPECT_FLOAT_EQ(24.0, deg90Data1.salt);
    EXPECT_FLOAT_EQ(23.6, deg90Data2.salt);
    EXPECT_FLOAT_EQ(27.0, deg90Data3.salt);
    EXPECT_FLOAT_EQ(26.8, deg90Data4.salt);

}


TEST(OceanFrontModelTest, Inside) {

    OceanFrontModel::Parameters parameters0Deg;
    parameters0Deg.frontOrientation = 0.0;
    parameters0Deg.frontWidth = 1000;
    parameters0Deg.frontX = 0;
    parameters0Deg.frontY = 0;
    parameters0Deg.depths = {0,5,10,20,30};
    parameters0Deg.side1Temps = {20,19,18,17,16};
    parameters0Deg.side2Temps = {20,18,16,14,12};
    parameters0Deg.side1Salts = {30,29,28,27,26};
    parameters0Deg.side2Salts = {30,28,26,24,22};
    parameters0Deg.currentU = 1;
    parameters0Deg.currentV = -1;
    parameters0Deg.dye = 2;

    OceanFrontModel model0Deg(parameters0Deg);

    OceanFrontModel::Parameters parameters90Deg;
    parameters90Deg.frontOrientation = -3.14159265359 / 2;
    parameters90Deg.frontWidth = 2000;
    parameters90Deg.frontX = 2000;
    parameters90Deg.frontY = -1000;
    parameters90Deg.depths = {0,5,10,20,30};
    parameters90Deg.side1Temps = {20,19,18,17,16};
    parameters90Deg.side2Temps = {20,18,16,14,12};
    parameters90Deg.side1Salts = {30,29,28,27,26};
    parameters90Deg.side2Salts = {30,28,26,24,22};

    OceanFrontModel model90Deg(parameters90Deg);

    ModelData deg0Data1 = model0Deg.getData(0, 250, 20, 0);
    ModelData deg0Data2 = model0Deg.getData(-5000, 250, 22, 0);
    ModelData deg0Data3 = model0Deg.getData(5000, -250, 20, 0);
    ModelData deg0Data4 = model0Deg.getData(10000, -250, 22, 0);

    EXPECT_FLOAT_EQ(14.75, deg0Data1.temp);
    EXPECT_FLOAT_EQ(14.4, deg0Data2.temp);
    EXPECT_FLOAT_EQ(16.25, deg0Data3.temp);
    EXPECT_FLOAT_EQ(16, deg0Data4.temp);

    EXPECT_FLOAT_EQ(24.75, deg0Data1.salt);
    EXPECT_FLOAT_EQ(24.4, deg0Data2.salt);
    EXPECT_FLOAT_EQ(26.25, deg0Data3.salt);
    EXPECT_FLOAT_EQ(26, deg0Data4.salt);

    ModelData deg90Data1 = model90Deg.getData(2500, 0, 20, 0);
    ModelData deg90Data2 = model90Deg.getData(2500, -5000, 22, 0);
    ModelData deg90Data3 = model90Deg.getData(1500, 5000, 20, 0);
    ModelData deg90Data4 = model90Deg.getData(1500, 10000, 22, 0);

    EXPECT_FLOAT_EQ(14.75, deg90Data1.temp);
    EXPECT_FLOAT_EQ(14.4, deg90Data2.temp);
    EXPECT_FLOAT_EQ(16.25, deg90Data3.temp);
    EXPECT_FLOAT_EQ(16, deg90Data4.temp);

    EXPECT_FLOAT_EQ(24.75, deg90Data1.salt);
    EXPECT_FLOAT_EQ(24.4, deg90Data2.salt);
    EXPECT_FLOAT_EQ(26.25, deg90Data3.salt);
    EXPECT_FLOAT_EQ(26, deg90Data4.salt);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
