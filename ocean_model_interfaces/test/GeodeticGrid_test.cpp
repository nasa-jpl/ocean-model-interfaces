#include "ocean_model_interfaces/geodetic_grid/GeodeticGrid.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

#include <gtest/gtest.h>

using namespace ocean_model_interfaces;

class GeodeticGridTest : public testing::Test {
protected:
    GeodeticGridTest() {
        GeodeticGridParameters parameters1;
        parameters1.modelDirectory = "./ocean_model_interfaces/test_data/geodetic_grid_test/";

        parameters1.timeChunkSize = 1;
        parameters1.depthChunkSize = 5;
        parameters1.latChunkSize = 6;
        parameters1.lonChunkSize = 6;

        model1 = GeodeticGrid(parameters1);

        GeodeticGridParameters parameters2;
        parameters2.modelDirectory = "./ocean_model_interfaces/test_data/geodetic_grid_test/";
        parameters2.timeChunkSize = 2;
        parameters2.depthChunkSize = 5;
        parameters2.latChunkSize = 6;
        parameters2.lonChunkSize = 6;

        model2 = GeodeticGrid(parameters2);
    }

    GeodeticGrid model1;
    GeodeticGrid model2;
};

TEST_F(GeodeticGridTest, IndexModelData)
{
    ModelData struct1StartData = model1.getDataAtIndex(0, 3, 4, 5);
    EXPECT_FLOAT_EQ(struct1StartData.u, 0.01317278016358614);
    EXPECT_FLOAT_EQ(struct1StartData.v, -4.7695075045339763E-4);
    EXPECT_FLOAT_EQ(struct1StartData.w, -0.0020381394);
    EXPECT_FLOAT_EQ(struct1StartData.temp, 0.95487386);
    EXPECT_FLOAT_EQ(struct1StartData.salt, 34.707146);
    EXPECT_FLOAT_EQ(struct1StartData.dye, 4.6685445E-17);
    EXPECT_FLOAT_EQ(struct1StartData.depth, 4192.582992535584);

    ModelData struct1EndData = model1.getDataAtIndex(1, 31, 48, 49);
    EXPECT_FLOAT_EQ(struct1EndData.u, -0.1458616405725479);
    EXPECT_FLOAT_EQ(struct1EndData.v, 0.05776486173272133);
    EXPECT_FLOAT_EQ(struct1EndData.w, -5.432432E-4);
    EXPECT_FLOAT_EQ(struct1EndData.temp, 29.887133);
    EXPECT_FLOAT_EQ(struct1EndData.salt, 35.90119);
    EXPECT_FLOAT_EQ(struct1EndData.dye, 2.4945718E-6);
    EXPECT_FLOAT_EQ(struct1EndData.depth, 4240.553816352657);

    ModelData struct1MidData = model1.getDataAtIndex(0, 14, 25, 36);
    EXPECT_FLOAT_EQ(struct1MidData.u, -0.029579374939203262);
    EXPECT_FLOAT_EQ(struct1MidData.v, -0.027591418474912643);
    EXPECT_FLOAT_EQ(struct1MidData.w, 0.0021991155);
    EXPECT_FLOAT_EQ(struct1MidData.temp, 1.6203395);
    EXPECT_FLOAT_EQ(struct1MidData.salt, 34.665573);
    EXPECT_FLOAT_EQ(struct1MidData.dye, 1.5467025E-8);
    EXPECT_FLOAT_EQ(struct1MidData.depth, 4260.392949210215);

    ModelData struct2StartData = model2.getDataAtIndex(0, 3, 4, 5);
    EXPECT_FLOAT_EQ(struct2StartData.u, 0.01317278016358614);
    EXPECT_FLOAT_EQ(struct2StartData.v, -4.7695075045339763E-4);
    EXPECT_FLOAT_EQ(struct2StartData.w, -0.0020381394);
    EXPECT_FLOAT_EQ(struct2StartData.temp, 0.95487386);
    EXPECT_FLOAT_EQ(struct2StartData.salt, 34.707146);
    EXPECT_FLOAT_EQ(struct2StartData.dye, 4.6685445E-17);
    EXPECT_FLOAT_EQ(struct2StartData.depth, 4192.582992535584);

    ModelData struct2EndData = model2.getDataAtIndex(1, 31, 48, 49);
    EXPECT_FLOAT_EQ(struct2EndData.u, -0.1458616405725479);
    EXPECT_FLOAT_EQ(struct2EndData.v, 0.05776486173272133);
    EXPECT_FLOAT_EQ(struct2EndData.w, -5.432432E-4);
    EXPECT_FLOAT_EQ(struct2EndData.temp, 29.887133);
    EXPECT_FLOAT_EQ(struct2EndData.salt, 35.90119);
    EXPECT_FLOAT_EQ(struct2EndData.dye, 2.4945718E-6);
    EXPECT_FLOAT_EQ(struct2EndData.depth, 4240.553816352657);

    ModelData struct2MidData = model2.getDataAtIndex(0, 14, 25, 36);
    EXPECT_FLOAT_EQ(struct2MidData.u, -0.029579374939203262);
    EXPECT_FLOAT_EQ(struct2MidData.v, -0.027591418474912643);
    EXPECT_FLOAT_EQ(struct2MidData.w, 0.0021991155);
    EXPECT_FLOAT_EQ(struct2MidData.temp, 1.6203395);
    EXPECT_FLOAT_EQ(struct2MidData.salt, 34.665573);
    EXPECT_FLOAT_EQ(struct2MidData.dye, 1.5467025E-8);
    EXPECT_FLOAT_EQ(struct2MidData.depth, 4260.392949210215);
}

TEST_F(GeodeticGridTest, GetModelData)
{
    ModelData data = model1.getData(-169.2590, -14.57603, -4177.89994465, 2506688.8);

    std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>,ModelData> indexData;
    indexData.insert({std::make_tuple(0,0,0,0), model1.getDataAtIndex(0,0,0,0)});
    indexData.insert({std::make_tuple(0,0,0,1), model1.getDataAtIndex(0,0,0,1)});
    indexData.insert({std::make_tuple(0,0,1,0), model1.getDataAtIndex(0,0,1,0)});
    indexData.insert({std::make_tuple(0,1,0,0), model1.getDataAtIndex(0,1,0,0)});
    indexData.insert({std::make_tuple(0,1,0,1), model1.getDataAtIndex(0,1,0,1)});
    indexData.insert({std::make_tuple(0,1,1,0), model1.getDataAtIndex(0,1,1,0)});
    indexData.insert({std::make_tuple(1,0,0,0), model1.getDataAtIndex(1,0,0,0)});
    indexData.insert({std::make_tuple(1,0,0,1), model1.getDataAtIndex(1,0,0,1)});
    indexData.insert({std::make_tuple(1,0,1,0), model1.getDataAtIndex(1,0,1,0)});
    indexData.insert({std::make_tuple(1,1,0,0), model1.getDataAtIndex(1,1,0,0)});
    indexData.insert({std::make_tuple(1,1,0,1), model1.getDataAtIndex(1,1,0,1)});
    indexData.insert({std::make_tuple(1,1,1,0), model1.getDataAtIndex(1,1,1,0)});

    double finalU = 0;
    finalU += indexData[std::make_tuple(0,0,0,0)].u * (0.27407083 * 0.8 * 0.7);
    finalU += indexData[std::make_tuple(0,0,0,1)].u * (0.416 * 0.8 * 0.7);
    finalU += indexData[std::make_tuple(0,0,1,0)].u * (0.30992916 * 0.8 * 0.7);
    finalU += indexData[std::make_tuple(0,1,0,0)].u * (0.27407083 * 0.2 * 0.7);
    finalU += indexData[std::make_tuple(0,1,0,1)].u * (0.416 * 0.2 * 0.7);
    finalU += indexData[std::make_tuple(0,1,1,0)].u * (0.30992916 * 0.2 * 0.7);
    finalU += indexData[std::make_tuple(1,0,0,0)].u * (0.27407083 * 0.8 * 0.3);
    finalU += indexData[std::make_tuple(1,0,0,1)].u * (0.416 * 0.8 * 0.3);
    finalU += indexData[std::make_tuple(1,0,1,0)].u * (0.30992916 * 0.8 * 0.3);
    finalU += indexData[std::make_tuple(1,1,0,0)].u * (0.27407083 * 0.2 * 0.3);
    finalU += indexData[std::make_tuple(1,1,0,1)].u * (0.416 * 0.2 * 0.3);
    finalU += indexData[std::make_tuple(1,1,1,0)].u * (0.30992916 * 0.2 * 0.3);

    double finalV = 0;
    finalV += indexData[std::make_tuple(0,0,0,0)].v * (0.27407083 * 0.8 * 0.7);
    finalV += indexData[std::make_tuple(0,0,0,1)].v * (0.416 * 0.8 * 0.7);
    finalV += indexData[std::make_tuple(0,0,1,0)].v * (0.30992916 * 0.8 * 0.7);
    finalV += indexData[std::make_tuple(0,1,0,0)].v * (0.27407083 * 0.2 * 0.7);
    finalV += indexData[std::make_tuple(0,1,0,1)].v * (0.416 * 0.2 * 0.7);
    finalV += indexData[std::make_tuple(0,1,1,0)].v * (0.30992916 * 0.2 * 0.7);
    finalV += indexData[std::make_tuple(1,0,0,0)].v * (0.27407083 * 0.8 * 0.3);
    finalV += indexData[std::make_tuple(1,0,0,1)].v * (0.416 * 0.8 * 0.3);
    finalV += indexData[std::make_tuple(1,0,1,0)].v * (0.30992916 * 0.8 * 0.3);
    finalV += indexData[std::make_tuple(1,1,0,0)].v * (0.27407083 * 0.2 * 0.3);
    finalV += indexData[std::make_tuple(1,1,0,1)].v * (0.416 * 0.2 * 0.3);
    finalV += indexData[std::make_tuple(1,1,1,0)].v * (0.30992916 * 0.2 * 0.3);

    double finalW = 0;
    finalW += indexData[std::make_tuple(0,0,0,0)].w * (0.27407083 * 0.8 * 0.7);
    finalW += indexData[std::make_tuple(0,0,0,1)].w * (0.416 * 0.8 * 0.7);
    finalW += indexData[std::make_tuple(0,0,1,0)].w * (0.30992916 * 0.8 * 0.7);
    finalW += indexData[std::make_tuple(0,1,0,0)].w * (0.27407083 * 0.2 * 0.7);
    finalW += indexData[std::make_tuple(0,1,0,1)].w * (0.416 * 0.2 * 0.7);
    finalW += indexData[std::make_tuple(0,1,1,0)].w * (0.30992916 * 0.2 * 0.7);
    finalW += indexData[std::make_tuple(1,0,0,0)].w * (0.27407083 * 0.8 * 0.3);
    finalW += indexData[std::make_tuple(1,0,0,1)].w * (0.416 * 0.8 * 0.3);
    finalW += indexData[std::make_tuple(1,0,1,0)].w * (0.30992916 * 0.8 * 0.3);
    finalW += indexData[std::make_tuple(1,1,0,0)].w * (0.27407083 * 0.2 * 0.3);
    finalW += indexData[std::make_tuple(1,1,0,1)].w * (0.416 * 0.2 * 0.3);
    finalW += indexData[std::make_tuple(1,1,1,0)].w * (0.30992916 * 0.2 * 0.3);

    double finalSalt = 0;
    finalSalt += indexData[std::make_tuple(0,0,0,0)].salt * (0.27407083 * 0.8 * 0.7);
    finalSalt += indexData[std::make_tuple(0,0,0,1)].salt * (0.416 * 0.8 * 0.7);
    finalSalt += indexData[std::make_tuple(0,0,1,0)].salt * (0.30992916 * 0.8 * 0.7);
    finalSalt += indexData[std::make_tuple(0,1,0,0)].salt * (0.27407083 * 0.2 * 0.7);
    finalSalt += indexData[std::make_tuple(0,1,0,1)].salt * (0.416 * 0.2 * 0.7);
    finalSalt += indexData[std::make_tuple(0,1,1,0)].salt * (0.30992916 * 0.2 * 0.7);
    finalSalt += indexData[std::make_tuple(1,0,0,0)].salt * (0.27407083 * 0.8 * 0.3);
    finalSalt += indexData[std::make_tuple(1,0,0,1)].salt * (0.416 * 0.8 * 0.3);
    finalSalt += indexData[std::make_tuple(1,0,1,0)].salt * (0.30992916 * 0.8 * 0.3);
    finalSalt += indexData[std::make_tuple(1,1,0,0)].salt * (0.27407083 * 0.2 * 0.3);
    finalSalt += indexData[std::make_tuple(1,1,0,1)].salt * (0.416 * 0.2 * 0.3);
    finalSalt += indexData[std::make_tuple(1,1,1,0)].salt * (0.30992916 * 0.2 * 0.3);

    double finalTemp = 0;
    finalTemp += indexData[std::make_tuple(0,0,0,0)].temp * (0.27407083 * 0.8 * 0.7);
    finalTemp += indexData[std::make_tuple(0,0,0,1)].temp * (0.416 * 0.8 * 0.7);
    finalTemp += indexData[std::make_tuple(0,0,1,0)].temp * (0.30992916 * 0.8 * 0.7);
    finalTemp += indexData[std::make_tuple(0,1,0,0)].temp * (0.27407083 * 0.2 * 0.7);
    finalTemp += indexData[std::make_tuple(0,1,0,1)].temp * (0.416 * 0.2 * 0.7);
    finalTemp += indexData[std::make_tuple(0,1,1,0)].temp * (0.30992916 * 0.2 * 0.7);
    finalTemp += indexData[std::make_tuple(1,0,0,0)].temp * (0.27407083 * 0.8 * 0.3);
    finalTemp += indexData[std::make_tuple(1,0,0,1)].temp * (0.416 * 0.8 * 0.3);
    finalTemp += indexData[std::make_tuple(1,0,1,0)].temp * (0.30992916 * 0.8 * 0.3);
    finalTemp += indexData[std::make_tuple(1,1,0,0)].temp * (0.27407083 * 0.2 * 0.3);
    finalTemp += indexData[std::make_tuple(1,1,0,1)].temp * (0.416 * 0.2 * 0.3);
    finalTemp += indexData[std::make_tuple(1,1,1,0)].temp * (0.30992916 * 0.2 * 0.3);

    double finalDye = 0;
    finalDye += indexData[std::make_tuple(0,0,0,0)].dye * (0.27407083 * 0.8 * 0.7);
    finalDye += indexData[std::make_tuple(0,0,0,1)].dye * (0.416 * 0.8 * 0.7);
    finalDye += indexData[std::make_tuple(0,0,1,0)].dye * (0.30992916 * 0.8 * 0.7);
    finalDye += indexData[std::make_tuple(0,1,0,0)].dye * (0.27407083 * 0.2 * 0.7);
    finalDye += indexData[std::make_tuple(0,1,0,1)].dye * (0.416 * 0.2 * 0.7);
    finalDye += indexData[std::make_tuple(0,1,1,0)].dye * (0.30992916 * 0.2 * 0.7);
    finalDye += indexData[std::make_tuple(1,0,0,0)].dye * (0.27407083 * 0.8 * 0.3);
    finalDye += indexData[std::make_tuple(1,0,0,1)].dye * (0.416 * 0.8 * 0.3);
    finalDye += indexData[std::make_tuple(1,0,1,0)].dye * (0.30992916 * 0.8 * 0.3);
    finalDye += indexData[std::make_tuple(1,1,0,0)].dye * (0.27407083 * 0.2 * 0.3);
    finalDye += indexData[std::make_tuple(1,1,0,1)].dye * (0.416 * 0.2 * 0.3);
    finalDye += indexData[std::make_tuple(1,1,1,0)].dye * (0.30992916 * 0.2 * 0.3);

    EXPECT_NEAR(data.u, finalU, 0.000001);
    EXPECT_NEAR(data.v, finalV, 0.000001);
    EXPECT_NEAR(data.w, finalW, 0.000001);
    EXPECT_NEAR(data.temp, finalTemp, 0.000001);
    EXPECT_NEAR(data.salt, finalSalt, 0.000001);
    EXPECT_NEAR(data.dye, finalDye, 0.000001);

    EXPECT_FLOAT_EQ(data.depth, 4196.58100612);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}