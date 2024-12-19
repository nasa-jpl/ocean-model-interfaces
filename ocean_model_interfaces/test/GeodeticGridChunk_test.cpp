#include "ocean_model_interfaces/geodetic_grid/GeodeticGridStructure.h"
#include "ocean_model_interfaces/geodetic_grid/GeodeticGridChunk.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

#include <gtest/gtest.h>

using namespace ocean_model_interfaces;

class GeodeticGridChunkTest : public testing::Test {
 protected:
  GeodeticGridChunkTest() {
    GeodeticGridParameters parameters1;
    parameters1.modelDirectory = "./ocean_model_interfaces/test_data/geodetic_grid_test/";

    parameters1.timeChunkSize = 1;
    parameters1.depthChunkSize = 5;
    parameters1.latChunkSize = 6;
    parameters1.lonChunkSize = 6;

    structure1 = GeodeticGridStructure(parameters1);

    GeodeticGridParameters parameters2;
    parameters2.modelDirectory = "./ocean_model_interfaces/test_data/geodetic_grid_test/";
    parameters2.timeChunkSize = 2;
    parameters2.depthChunkSize = 5;
    parameters2.latChunkSize = 6;
    parameters2.lonChunkSize = 6;

    structure2 = GeodeticGridStructure(parameters2);
  }

  GeodeticGridStructure structure1;
  GeodeticGridStructure structure2;

};

TEST_F(GeodeticGridChunkTest, GetGridChunkInfo)
{
    GeodeticGridStructure::ChunkInfo struct1Start = structure1.getGridChunkInfo(0, 3, 4, 5);
    GeodeticGridChunk struct1StartChunk(struct1Start, structure1.getModelFiles());
    ModelData struct1StartData = struct1StartChunk.getData(0, 3, 4, 5);
    EXPECT_FLOAT_EQ(struct1StartData.u, 0.01317278016358614);
    EXPECT_FLOAT_EQ(struct1StartData.v, -4.7695075045339763E-4);
    EXPECT_FLOAT_EQ(struct1StartData.w, -0.0020381394);
    EXPECT_FLOAT_EQ(struct1StartData.temp, 0.95487386);
    EXPECT_FLOAT_EQ(struct1StartData.salt, 34.707146);
    EXPECT_FLOAT_EQ(struct1StartData.dye, 4.6685445E-17);

    GeodeticGridStructure::ChunkInfo struct1End = structure1.getGridChunkInfo(1, 31, 48, 49);
    GeodeticGridChunk struct1EndChunk(struct1End, structure1.getModelFiles());
    ModelData struct1EndData = struct1EndChunk.getData(1, 31, 48, 49);
    EXPECT_FLOAT_EQ(struct1EndData.u, -0.1458616405725479);
    EXPECT_FLOAT_EQ(struct1EndData.v, 0.05776486173272133);
    EXPECT_FLOAT_EQ(struct1EndData.w, -5.432432E-4);
    EXPECT_FLOAT_EQ(struct1EndData.temp, 29.887133);
    EXPECT_FLOAT_EQ(struct1EndData.salt, 35.90119);
    EXPECT_FLOAT_EQ(struct1EndData.dye, 2.4945718E-6);

    GeodeticGridStructure::ChunkInfo struct1Mid = structure1.getGridChunkInfo(0, 14, 25, 36);
    GeodeticGridChunk struct1MidChunk(struct1Mid, structure1.getModelFiles());
    ModelData struct1MidData = struct1MidChunk.getData(0, 14, 25, 36);
    EXPECT_FLOAT_EQ(struct1MidData.u, -0.029579374939203262);
    EXPECT_FLOAT_EQ(struct1MidData.v, -0.027591418474912643);
    EXPECT_FLOAT_EQ(struct1MidData.w, 0.0021991155);
    EXPECT_FLOAT_EQ(struct1MidData.temp, 1.6203395);
    EXPECT_FLOAT_EQ(struct1MidData.salt, 34.665573);
    EXPECT_FLOAT_EQ(struct1MidData.dye, 1.5467025E-8);

    GeodeticGridStructure::ChunkInfo struct2Start = structure2.getGridChunkInfo(0, 3, 4, 5);
    GeodeticGridChunk struct2StartChunk(struct2Start, structure2.getModelFiles());
    ModelData struct2StartData = struct2StartChunk.getData(0, 3, 4, 5);
    EXPECT_FLOAT_EQ(struct2StartData.u, 0.01317278016358614);
    EXPECT_FLOAT_EQ(struct2StartData.v, -4.7695075045339763E-4);
    EXPECT_FLOAT_EQ(struct2StartData.w, -0.0020381394);
    EXPECT_FLOAT_EQ(struct2StartData.temp, 0.95487386);
    EXPECT_FLOAT_EQ(struct2StartData.salt, 34.707146);
    EXPECT_FLOAT_EQ(struct2StartData.dye, 4.6685445E-17);

    GeodeticGridStructure::ChunkInfo struct2End = structure2.getGridChunkInfo(1, 31, 48, 49);
    GeodeticGridChunk struct2EndChunk(struct2End, structure2.getModelFiles());
    ModelData struct2EndData = struct2EndChunk.getData(1, 31, 48, 49);
    EXPECT_FLOAT_EQ(struct2EndData.u, -0.1458616405725479);
    EXPECT_FLOAT_EQ(struct2EndData.v, 0.05776486173272133);
    EXPECT_FLOAT_EQ(struct2EndData.w, -5.432432E-4);
    EXPECT_FLOAT_EQ(struct2EndData.temp, 29.887133);
    EXPECT_FLOAT_EQ(struct2EndData.salt, 35.90119);
    EXPECT_FLOAT_EQ(struct2EndData.dye, 2.4945718E-6);

    GeodeticGridStructure::ChunkInfo struct2Mid = structure2.getGridChunkInfo(0, 14, 25, 36);
    GeodeticGridChunk struct2MidChunk(struct2Mid, structure2.getModelFiles());
    ModelData struct2MidData = struct2MidChunk.getData(0, 14, 25, 36);
    EXPECT_FLOAT_EQ(struct2MidData.u, -0.029579374939203262);
    EXPECT_FLOAT_EQ(struct2MidData.v, -0.027591418474912643);
    EXPECT_FLOAT_EQ(struct2MidData.w, 0.0021991155);
    EXPECT_FLOAT_EQ(struct2MidData.temp, 1.6203395);
    EXPECT_FLOAT_EQ(struct2MidData.salt, 34.665573);
    EXPECT_FLOAT_EQ(struct2MidData.dye, 1.5467025E-8);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}