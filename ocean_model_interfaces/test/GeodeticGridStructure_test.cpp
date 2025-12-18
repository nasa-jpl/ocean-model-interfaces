#include "ocean_model_interfaces/geodetic_grid/GeodeticGridStructure.h"
#include <gtest/gtest.h>

using namespace ocean_model_interfaces;

class GeodeticGridStructureTest : public testing::Test {
 protected:
  GeodeticGridStructureTest() {
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

TEST_F(GeodeticGridStructureTest, GetGridChunkInfo)
{
    //Do one chunk at start, one in middle, one at end for each structure

    //The first chunk in structure1
    GeodeticGridStructure::ChunkInfo struct1Start = structure1.getGridChunkInfo(0, 3, 4, 5);
    EXPECT_EQ(struct1Start.id, 0);
    EXPECT_EQ(struct1Start.timeChunk, 0);
    EXPECT_EQ(struct1Start.depthChunk, 0);
    EXPECT_EQ(struct1Start.latChunk, 0);
    EXPECT_EQ(struct1Start.lonChunk, 0);
    EXPECT_EQ(struct1Start.timeStart, 0);
    EXPECT_EQ(struct1Start.depthStart, 0);
    EXPECT_EQ(struct1Start.latStart, 0);
    EXPECT_EQ(struct1Start.lonStart, 0);
    EXPECT_EQ(struct1Start.timeSize, 1);
    EXPECT_EQ(struct1Start.depthSize, 5);
    EXPECT_EQ(struct1Start.latSize, 6);
    EXPECT_EQ(struct1Start.lonSize, 6);

    //The last chunk in structure1
    GeodeticGridStructure::ChunkInfo struct1End = structure1.getGridChunkInfo(1, 31, 48, 49);
    EXPECT_EQ(struct1End.id, 1133);
    EXPECT_EQ(struct1End.timeChunk, 1);
    EXPECT_EQ(struct1End.depthChunk, 6);
    EXPECT_EQ(struct1End.latChunk, 8);
    EXPECT_EQ(struct1End.lonChunk, 8);
    EXPECT_EQ(struct1End.timeStart, 1);
    EXPECT_EQ(struct1End.depthStart, 30);
    EXPECT_EQ(struct1End.latStart, 48);
    EXPECT_EQ(struct1End.lonStart , 48);
    EXPECT_EQ(struct1End.timeSize, 1); //Sizes are smaller because it's the last chunk so it's just whatever is left
    EXPECT_EQ(struct1End.depthSize, 2);
    EXPECT_EQ(struct1End.latSize, 2);
    EXPECT_EQ(struct1End.lonSize, 2);

    //A Middle chunk from structure1
    GeodeticGridStructure::ChunkInfo struct1Mid = structure1.getGridChunkInfo(0, 14, 25, 36);
    EXPECT_EQ(struct1Mid.id, 204);
    EXPECT_EQ(struct1Mid.timeChunk, 0);
    EXPECT_EQ(struct1Mid.depthChunk, 2);
    EXPECT_EQ(struct1Mid.latChunk, 4);
    EXPECT_EQ(struct1Mid.lonChunk, 6);
    EXPECT_EQ(struct1Mid.timeStart, 0);
    EXPECT_EQ(struct1Mid.depthStart, 10);
    EXPECT_EQ(struct1Mid.latStart, 24);
    EXPECT_EQ(struct1Mid.lonStart, 36);
    EXPECT_EQ(struct1Mid.timeSize, 1);
    EXPECT_EQ(struct1Mid.depthSize, 5);
    EXPECT_EQ(struct1Mid.latSize, 6);
    EXPECT_EQ(struct1Mid.lonSize, 6);


    //The first chunk in structure2
    GeodeticGridStructure::ChunkInfo struct2Start = structure2.getGridChunkInfo(0, 3, 4, 5);
    EXPECT_EQ(struct2Start.id, 0);
    EXPECT_EQ(struct2Start.timeChunk, 0);
    EXPECT_EQ(struct2Start.depthChunk, 0);
    EXPECT_EQ(struct2Start.latChunk, 0);
    EXPECT_EQ(struct2Start.lonChunk, 0);
    EXPECT_EQ(struct2Start.timeStart, 0);
    EXPECT_EQ(struct2Start.depthStart, 0);
    EXPECT_EQ(struct2Start.latStart, 0);
    EXPECT_EQ(struct2Start.lonStart, 0);
    EXPECT_EQ(struct2Start.timeSize, 2);
    EXPECT_EQ(struct2Start.depthSize, 5);
    EXPECT_EQ(struct2Start.latSize, 6);
    EXPECT_EQ(struct2Start.lonSize, 6);

    //The last chunk in structure2
    GeodeticGridStructure::ChunkInfo struct2End = structure2.getGridChunkInfo(1, 31, 48, 49);
    EXPECT_EQ(struct2End.id, 566);
    EXPECT_EQ(struct2End.timeChunk, 0);
    EXPECT_EQ(struct2End.depthChunk, 6);
    EXPECT_EQ(struct2End.latChunk, 8);
    EXPECT_EQ(struct2End.lonChunk, 8);
    EXPECT_EQ(struct2End.timeStart, 0);
    EXPECT_EQ(struct2End.depthStart, 30);
    EXPECT_EQ(struct2End.latStart, 48);
    EXPECT_EQ(struct2End.lonStart , 48);
    EXPECT_EQ(struct2End.timeSize, 2); //Sizes are smaller because it's the last chunk so it's just whatever is left
    EXPECT_EQ(struct2End.depthSize, 2);
    EXPECT_EQ(struct2End.latSize, 2);
    EXPECT_EQ(struct2End.lonSize, 2);

    //A Middle chunk from structure2
    GeodeticGridStructure::ChunkInfo struct2Mid = structure2.getGridChunkInfo(0, 14, 25, 36);
    EXPECT_EQ(struct2Mid.id, 204);
    EXPECT_EQ(struct2Mid.timeChunk, 0);
    EXPECT_EQ(struct2Mid.depthChunk, 2);
    EXPECT_EQ(struct2Mid.latChunk, 4);
    EXPECT_EQ(struct2Mid.lonChunk, 6);
    EXPECT_EQ(struct2Mid.timeStart, 0);
    EXPECT_EQ(struct2Mid.depthStart, 10);
    EXPECT_EQ(struct2Mid.latStart, 24);
    EXPECT_EQ(struct2Mid.lonStart, 36);
    EXPECT_EQ(struct2Mid.timeSize, 2);
    EXPECT_EQ(struct2Mid.depthSize, 5);
    EXPECT_EQ(struct2Mid.latSize, 6);
    EXPECT_EQ(struct2Mid.lonSize, 6);

}

TEST_F(GeodeticGridStructureTest, IsInRange)
{
    EXPECT_TRUE(structure1.indexInRange(0,0,0,0));
    EXPECT_TRUE(structure1.indexInRange(1, 31, 49, 49));
    EXPECT_FALSE(structure1.indexInRange(2, 31, 49, 49));
    EXPECT_FALSE(structure1.indexInRange(1, 32, 49, 49));
    EXPECT_FALSE(structure1.indexInRange(1, 31, 50, 49));
    EXPECT_FALSE(structure1.indexInRange(1, 31, 49, 50));
}

TEST_F(GeodeticGridStructureTest, TimeInModel)
{
    EXPECT_FALSE(structure1.timeInModel(2506504.0 - 1));
    EXPECT_TRUE(structure1.timeInModel(2506504.0 + 1));

    EXPECT_TRUE(structure1.timeInModel(2507120.0 - 1));
    EXPECT_FALSE(structure1.timeInModel(2507120.0 + 1));
}


TEST_F(GeodeticGridStructureTest, DepthInModel)
{
    EXPECT_TRUE(structure1.depthInModel(Point(-169.2590, -14.57603, -4196.58100612 + 1)));
    EXPECT_FALSE(structure1.depthInModel(Point(-169.2590, -14.57603, -4196.58100612 - 1)));
}

TEST_F(GeodeticGridStructureTest, XYInModel)
{
    EXPECT_FALSE(structure1.xyInModel(Point(-169.2593537414966 - 0.00001, -14.576293545209538 + 0.00001, 0)));
    EXPECT_FALSE(structure1.xyInModel(Point(-169.2593537414966 + 0.00001, -14.576293545209538 - 0.00001, 0)));
    EXPECT_TRUE(structure1.xyInModel(Point(-169.2593537414966 + 0.00001, -14.576293545209538 + 0.00001, 0)));

    EXPECT_TRUE(structure1.xyInModel(Point(-169.21768707482994 - 0.000001, -14.53462687854287 - 0.00001, 0)));
    EXPECT_FALSE(structure1.xyInModel(Point(-169.21768707482994 - 0.000001, -14.53462687854287 + 0.00001, 0)));
    EXPECT_FALSE(structure1.xyInModel(Point(-169.21768707482994 + 0.000001, -14.53462687854287 - 0.00001, 0)));
}


TEST_F(GeodeticGridStructureTest, GetWaterColumnDepth)
{
    EXPECT_DOUBLE_EQ(structure1.indexWaterColumnDepth(4, 5), 4192.582992535584);
    EXPECT_DOUBLE_EQ(structure1.indexWaterColumnDepth(48, 49), 4240.553816352657);
    EXPECT_DOUBLE_EQ(structure1.indexWaterColumnDepth(25, 36), 4260.392949210215);
}

TEST_F(GeodeticGridStructureTest, GetDataInterpolationWeights)
{
    //Test directly on Index 0,10,25,25
    Point onNode1 = Point(-169.23809523809524, -14.555035041808177, -3546.6216406943277);
    double onNode1Time = 2506504.0;
    auto onNode1Weights = structure1.getDataInterpolationWeights(onNode1, onNode1Time);
    for(auto const& weight : onNode1Weights) {
        if(weight.first == std::make_tuple(0,10,25,25)) {
            EXPECT_DOUBLE_EQ(weight.second, 1.0);
        } else {
            EXPECT_DOUBLE_EQ(weight.second, 0.0);
        }
  }


    //Test directly on Index 0,[depth value 0],0,0
    Point onNode2 = Point(-169.2593537414966, -14.576293545209538, -4196.519895424129);
    double onNodeTime2 = 2506504.0;
    auto onNode2Weights = structure1.getDataInterpolationWeights(onNode2, onNodeTime2);
    for(auto const& weight : onNode2Weights) {
        if(weight.first == std::make_tuple(0,0,0,0)) {
            EXPECT_DOUBLE_EQ(weight.second, 1.0);
        } else {
            EXPECT_DOUBLE_EQ(weight.second, 0.0);
        }
    }

    //Test directly on Index 1,[depth value == 0]],49,49
    Point onNode3 = Point(-169.21768707482994, -14.53462687854287, 0);
    double onNode3Time = 2507120.0;
    auto onNode3Weights = structure1.getDataInterpolationWeights(onNode3, onNode3Time);
    for(auto const& weight : onNode3Weights) {
        //std::cout << std::get<0>(weight.first) << " "  << std::get<1>(weight.first) << " "  << std::get<2>(weight.first) << " "  << std::get<3>(weight.first) << std::endl;
        if(weight.first == std::make_tuple(1,31,49,49)) {
            EXPECT_DOUBLE_EQ(weight.second, 1.0);
        } else {
            EXPECT_DOUBLE_EQ(weight.second, 0.0);
        }
    }

    //Test on node [depth value == water column depth],0,0, but between index 0 and 1 on time
    Point betweenTime = Point(-169.2593537414966, -14.576293545209538, -4196.519895424129);
    double betweenTimeTime = 2506688.8;
    auto betweenTimeWeights = structure1.getDataInterpolationWeights(betweenTime, betweenTimeTime);
    for(auto const& weight : betweenTimeWeights) {
        if(weight.first == std::make_tuple(0,0,0,0)) {
            EXPECT_FLOAT_EQ(weight.second, 0.7);
        } else if(weight.first == std::make_tuple(1,0,0,0)){
            EXPECT_FLOAT_EQ(weight.second, 0.3);
        } else {
            EXPECT_FLOAT_EQ(weight.second, 0.0);
        }
    }

    //Test on node 0,[between depths],0,0
    Point betweenDepths = Point(-169.2593537414966, -14.576293545209538, -4174.92410497);
    double betweenDepthsTime = 2506504.0;
    auto betweenDepthsWeights = structure1.getDataInterpolationWeights(betweenDepths, betweenDepthsTime);
    for(auto const& weight : betweenDepthsWeights) {
        if(weight.first == std::make_tuple(0,0,0,0)) {
            EXPECT_FLOAT_EQ(weight.second, 0.7);
        } else if(weight.first == std::make_tuple(0,1,0,0)){
            EXPECT_FLOAT_EQ(weight.second, 0.3);
        } else {
            EXPECT_FLOAT_EQ(weight.second, 0.0);
        }
    }

    //Test on node 0,[at the seafloor], and between lat,lon nodes
    Point betweenXY = Point(-169.2590, -14.57603, -4190);
    double betweenXYTime = 2506504.0;
    auto betweenXYWeights = structure1.getDataInterpolationWeights(betweenXY, betweenXYTime);
    for(auto const& weight : betweenXYWeights) {
        if(weight.first == std::make_tuple(0,0,0,0)) {
            EXPECT_FLOAT_EQ(weight.second, 0.27407083);
        } else if(weight.first == std::make_tuple(0,0,0,1)){
            EXPECT_FLOAT_EQ(weight.second, 0.416);
        } else if(weight.first == std::make_tuple(0,0,1,0)){
            EXPECT_FLOAT_EQ(weight.second, 0.30992916);
        } else {
            EXPECT_FLOAT_EQ(weight.second, 0.0);
        }
    }


    Point betweenAll = Point(-169.2590, -14.57603, -4177.89994465);
    double betweenAllTime = 2506688.8;
    auto betweenAllWeights = structure1.getDataInterpolationWeights(betweenAll, betweenAllTime);
    for(auto const& weight : betweenAllWeights) {
        if(weight.first == std::make_tuple(0,0,0,0)) {
            EXPECT_NEAR(weight.second, (0.27407083 * 0.8 * 0.7), 0.001);
        } else if(weight.first == std::make_tuple(0,0,0,1)) {
            EXPECT_NEAR(weight.second, (0.416 * 0.8 * 0.7), 0.001);
        } else if(weight.first == std::make_tuple(0,0,1,0)) {
            EXPECT_NEAR(weight.second, (0.30992916 * 0.8 * 0.7), 0.001);
        } else if(weight.first == std::make_tuple(0,1,0,0)) {
            EXPECT_NEAR(weight.second, (0.27407083 * 0.2 * 0.7), 0.001);
        } else if(weight.first == std::make_tuple(0,1,0,1)) {
            EXPECT_NEAR(weight.second, (0.416 * 0.2 * 0.7), 0.001);
        } else if(weight.first == std::make_tuple(0,1,1,0)) {
            EXPECT_NEAR(weight.second, (0.30992916 * 0.2 * 0.7), 0.001);
        } else if(weight.first == std::make_tuple(1,0,0,0)) {
            EXPECT_NEAR(weight.second, (0.27407083 * 0.8 * 0.3), 0.001);
        } else if(weight.first == std::make_tuple(1,0,0,1)) {
            EXPECT_NEAR(weight.second, (0.416 * 0.8 * 0.3), 0.001);
        } else if(weight.first == std::make_tuple(1,0,1,0)) {
            EXPECT_NEAR(weight.second, (0.30992916 * 0.8 * 0.3), 0.001);
        } else if(weight.first == std::make_tuple(1,1,0,0)) {
            EXPECT_NEAR(weight.second, (0.27407083 * 0.2 * 0.3), 0.001);
        } else if(weight.first == std::make_tuple(1,1,0,1)) {
            EXPECT_NEAR(weight.second, (0.416 * 0.2 * 0.3), 0.001);
        } else if(weight.first == std::make_tuple(1,1,1,0)) {
            EXPECT_NEAR(weight.second, (0.30992916 * 0.2 * 0.3), 0.001);
        }
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}