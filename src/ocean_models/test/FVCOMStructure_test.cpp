#include "fvcom/FVCOMStructure.h"
#include <gtest/gtest.h>

FVCOMStructure structure("../../src/ocean_models/test_data/box_plume_split", 10, 10, 10, 10);
FVCOMStructure structureAxial("../../src/ocean_models/test_data/axial_data_test", 1000, 1000, 10, 10);

TEST(FVCOMStructureTest, InterpolateTime)
{

	int time1IndexExact = 0;
	int time2IndexExact = 0;
	double time1PercentExact = 0.0;
	structureAxial.timeInterpolation(0.125, time1IndexExact, time2IndexExact, time1PercentExact);


	//Inbetween indicies
	int time1IndexBetween = 0;
	int time2IndexBetween = 0;
	double time1PercentBetween = 0.0;
	structureAxial.timeInterpolation(0.22, time1IndexBetween, time2IndexBetween, time1PercentBetween);

	//Exactly on index
	ASSERT_EQ(3, time1IndexExact);
	ASSERT_EQ(3, time2IndexExact);
	ASSERT_FLOAT_EQ(1.0, time1PercentExact);

	//Inbetween indicies
	ASSERT_EQ(5, time1IndexBetween);
	ASSERT_EQ(6, time2IndexBetween);
	ASSERT_FLOAT_EQ(0.72, time1PercentBetween);
}

TEST(FVCOMStructureTest, GetTriangleSiglayPlane)
{
	//100
	//51, 290, 291
	//h: 3200.0, 3200.0, 3200.0
	// (-150000.0, -105000.0), (-145675.094, -103157.913), (-145675.008, -108157.064)
	//siglay 0: -0.003937007859349251
	//siglay 1: -0.027559055015444756
	FVCOMStructure::Plane plane0 = structureAxial.getTriangleSiglayPlane(100, 0);
	FVCOMStructure::Plane plane1 = structureAxial.getTriangleSiglayPlane(100, 3);

	ASSERT_FLOAT_EQ(0, plane0.a);
	ASSERT_FLOAT_EQ(0, plane0.b);
	ASSERT_FLOAT_EQ(-1, plane0.c);
	ASSERT_FLOAT_EQ(-12.5984251499, plane0.d);

	ASSERT_FLOAT_EQ(0, plane1.a);
	ASSERT_FLOAT_EQ(0, plane1.b);
	ASSERT_FLOAT_EQ(-1, plane1.c);
	ASSERT_FLOAT_EQ(-88.1889760494, plane1.d);
}

TEST(FVCOMStructureTest, CreatePlane)
{
	FVCOMStructure::Point p0;
	FVCOMStructure::Point p1;
	FVCOMStructure::Point p2;

	p0.x = 10;
	p0.y = 10;
	p0.h = -10;
	p1.x = -5;
	p1.y = -3;
	p1.h = -5;
	p2.x = -1;
	p2.y = -7;
	p2.h = -2;

	//v1: 15,13,-5
	//v2: 11,17,-8
	//cross: -19; 65; 112
	//cross normalized: -0.235244, 0.794745, -0.5595
	FVCOMStructure::Plane plane0(p0, p1, p2);
	
	ASSERT_FLOAT_EQ(-0.14516935, plane0.a);
	ASSERT_FLOAT_EQ(0.496632, plane0.b);
	ASSERT_FLOAT_EQ(0.855735, plane0.c);
	ASSERT_FLOAT_EQ(5.0427235, plane0.d);
}


TEST(FVCOMStructureTest, InterpolateSiglay)
{

	//Above first siglay
	FVCOMStructure::Point p0;
	p0.x = 0;
	p0.y = 0;
	p0.h = -5;

	int siglay1IndexP0 = 1;
	int siglay2IndexP0 = 1;
	double siglay1PercentP0 = 0;

	structureAxial.siglayInterpolation(p0, siglay1IndexP0, siglay2IndexP0, siglay1PercentP0);

	//Below last siglay
	FVCOMStructure::Point p1;
	p1.x = 50000.0;
	p1.y = -150000.0;
	p1.h = -2798;

	int siglay1IndexP1 = 0;
	int siglay2IndexP1 = 0;
	double siglay1PercentP1 = 0;

	structureAxial.siglayInterpolation(p1, siglay1IndexP1, siglay2IndexP1, siglay1PercentP1);


	ASSERT_EQ(29, siglay1IndexP1);
	ASSERT_EQ(29, siglay2IndexP1);
	ASSERT_FLOAT_EQ(1.0, siglay1PercentP1);


	//On siglay
	FVCOMStructure::Point p2;
	p2.x = 0;
	p2.y = 0;
	p2.h = 0;

	int siglay1IndexP2 = 0;
	int siglay2IndexP2 = 0;
	double siglay1PercentP2 = 0;

	
	int triangle2 = structureAxial.getContainingTriangle(p2);
	FVCOMStructure::Plane plane2 = structureAxial.getTriangleSiglayPlane(triangle2, 18);
	p2.h = (-plane2.d - plane2.a * p2.x - plane2.b * p2.y) / plane2.c;

	structureAxial.siglayInterpolation(p2, siglay1IndexP2, siglay2IndexP2, siglay1PercentP2);

	ASSERT_EQ(18, siglay1IndexP2);
	ASSERT_EQ(18, siglay2IndexP2);
	ASSERT_FLOAT_EQ(1.0, siglay1PercentP1);


	//Middle siglay	
	//6781, 6783, 6782 (index)
	//1555.529154
	// siglay 7,8
	FVCOMStructure::Point p3;
	p3.x = 0;
	p3.y = 0;
	p3.h = -100;

	int siglay1IndexP3 = 0;
	int siglay2IndexP3 = 0;
	double siglay1PercentP3 = 0;

	structureAxial.siglayInterpolation(p3, siglay1IndexP3, siglay2IndexP3, siglay1PercentP3);

	FVCOMStructure::Plane plane3a = structureAxial.getTriangleSiglayPlane(13325, 7);
	FVCOMStructure::Plane plane3b = structureAxial.getTriangleSiglayPlane(13325, 8);

	double upperH = (-plane3a.d - plane3a.a * p3.x - plane3a.b * p3.y) / plane3a.c;
	double lowerH = (-plane3b.d - plane3b.a * p3.x - plane3b.b * p3.y) / plane3b.c;



	ASSERT_EQ(7, siglay1IndexP3);
	ASSERT_EQ(8, siglay2IndexP3);
	ASSERT_DOUBLE_EQ((lowerH - p3.h) / (lowerH - upperH), siglay1PercentP3);
}


TEST(FVCOMStructureTest, GetClosestTime) {
	int timeExact1 = structure.getClosestTime(0.0020833334); //Index 1
	int timeExact2 = structure.getClosestTime(0.05); //Index 24

	int timeNotExact1 = structure.getClosestTime(0.048957333); //Index 23
	int timeNotExact2 = structure.getClosestTime(0.048959333); //Index 24

	int timeSmallerThanZero = structure.getClosestTime(-0.1);
	int timeGreaterThanMax = structure.getClosestTime(1);


	ASSERT_EQ(1, timeExact1);
	ASSERT_EQ(24, timeExact2);

	ASSERT_EQ(23, timeNotExact1);
	ASSERT_EQ(24, timeNotExact2);

	ASSERT_EQ(0, timeSmallerThanZero);
	ASSERT_EQ(80, timeGreaterThanMax);
}


TEST(FCVOMStructureTest, GetClosestNodeSiglay) {
	FVCOMStructure::Point p1;
	FVCOMStructure::Point p2;
	FVCOMStructure::Point p3;

	//Node 51
	//h = 300
	//siglay = 0
	p1.x = 70;
	p1.y = -100;
	p1.h = 0;

	//Node 51
	//h = 300
	//siglay = 69
	p2.x = 70;
	p2.y = -100;
	p2.h = -210.606051;

	//Node 51
	//h = 300
	//siglay = 70
	p3.x = 70;
	p3.y = -100;
	p3.h = -212.1212905;


	int surfaceExact = structure.getClosestNodeSiglay(p1);
	int depthExact = structure.getClosestNodeSiglay(p2);

	int depthNotExact = structure.getClosestNodeSiglay(p3);

	ASSERT_EQ(0, surfaceExact);
	ASSERT_EQ(69, depthExact);
	ASSERT_EQ(70, depthNotExact);
}


TEST(FCVOMStructureTest, GetClosestTriangleSiglay) {
	FVCOMStructure::Point p1;
	FVCOMStructure::Point p2;
	FVCOMStructure::Point p3;

	//Node 81
	//h = 300
	//siglay = 0
	p1.x = -15.735039;
	p1.y = -17.163147;
	p1.h = 0;

	//Node 81
	//h = 300
	//siglay = 69
	p2.x = -15.735039;
	p2.y = -17.163147;
	p2.h = -210.606051;

	//Node 81
	//h = 300
	//siglay = 70
	p3.x = -15.735039;
	p3.y = -17.163147;
	p3.h = -212.1212905;


	int surfaceExact = structure.getClosestTriangleSiglay(p1);
	int depthExact = structure.getClosestTriangleSiglay(p2);

	int depthNotExact = structure.getClosestTriangleSiglay(p3);

	ASSERT_EQ(0, surfaceExact);
	ASSERT_EQ(69, depthExact);
	ASSERT_EQ(70, depthNotExact);
}

TEST(FVCOMStructureTest, PointInTriangle) {
	FVCOMStructure::Point pIn;
	FVCOMStructure::Point pOut;
	FVCOMStructure::Point pEdge;


	//Triangle 1
	////Nodes: 385, 329,342
	//////X,Y 385: -48.038475, 10
	//////X,Y 329: -56.69873, 5
	//////X,Y 342: -56.69873, 15.0
	FVCOMStructure::Point node385 = structure.getNodePoint(385); //Get the exact x and y for edge point
	pEdge.x = node385.x;
	pEdge.y = node385.y;
	pOut.x = 0;
	pOut.y = 0;
	pIn.x = -50;
	pIn.y = 10;



	bool pInResults = structure.pointInTriangle(pIn, 1);
	bool pOutResults = structure.pointInTriangle(pOut, 1);
	bool pEdgeResults = structure.pointInTriangle(pEdge, 1);


	ASSERT_TRUE(pInResults);
	ASSERT_FALSE(pOutResults);
	ASSERT_TRUE(pEdgeResults);
}

TEST(FVCOMStructureTest, GetContainingTriangle) {
	FVCOMStructure::Point pIn;
	FVCOMStructure::Point pOut;
	FVCOMStructure::Point pEdge;

	//Triangle 1
	////Nodes: 385, 329,342
	//////X,Y 385: -48.038475, 10
	//////X,Y 329: -56.69873, 5
	//////X,Y 342: -56.69873, 15.0
	FVCOMStructure::Point node385 = structure.getNodePoint(385); //Get the exact x and y for edge point
	pEdge.x = node385.x;
	pEdge.y = node385.y;
	pOut.x = 0;
	pOut.y = 0;
	pIn.x = -50;
	pIn.y = 10;


	int pInResults = structure.getContainingTriangle(pIn);
	int pOutResults = structure.getContainingTriangle(pOut);
	int pEdgeResults = structure.getContainingTriangle(pEdge);

	ASSERT_EQ(1, pInResults);
	ASSERT_EQ(1, pEdgeResults);
	ASSERT_NE(1, pOutResults);
}


TEST(FVCOMStructureTest, GetClosestNode) {
	FVCOMStructure::Point p1;
	FVCOMStructure::Point p2;
	FVCOMStructure::Point p3;

	//Triangle 1
	////Nodes: 385, 329,342
	//////X,Y 385: -48.038475, 10
	//////X,Y 329: -56.69873, 5
	//////X,Y 342: -56.69873, 15.0
	p1.x = -48.038475;
	p1.y = 10;
	p2.x = -56.69873;
	p2.y = 10;
	p3.x = -57;
	p3.y = 6;

	int p1Results = structure.getClosestNode(p1);
	int p2Results = structure.getClosestNode(p2);
	int p3Results = structure.getClosestNode(p3);


	ASSERT_EQ(385, p1Results);
	ASSERT_EQ(329, p2Results);
	ASSERT_EQ(329, p3Results);
}


TEST(FVCOMStructureTest, Distance) {
	FVCOMStructure::Point p1;
	FVCOMStructure::Point p2;
	FVCOMStructure::Point p3;

	p1.x = 0;
	p1.y = 0;
	p2.x = 5;
	p2.y = 0;
	p3.x = 0;
	p3.y = 7;

	ASSERT_FLOAT_EQ(5.0, structure.distance(p1, p2));
	ASSERT_FLOAT_EQ(5.0, structure.distance(p2, p1));

	ASSERT_FLOAT_EQ(7.0, structure.distance(p1, p3));
	ASSERT_FLOAT_EQ(7.0, structure.distance(p3, p1));

	ASSERT_FLOAT_EQ(8.60232526704, structure.distance(p3, p2));
	ASSERT_FLOAT_EQ(8.60232526704, structure.distance(p2, p3));
}


TEST(FVCOMStructureTest, GetChunkForNode) {

	//model extent
	////x: -100 , 100
	////y: -100 , 100
	////siglay: 99
	////time: 81

	////Nodes: 385
	//////X,Y 385: -48.038475, 10

	//x Chunk: 5
	//y Chunk: 11
	//siglay: 0
	//time: 0
	//CHUNK: 9990

	//siglay: 0
	//time: 80
	//CHUNK: 9998

	//siglay: 54
	//time: 80
	//CHUNK: 10043

	FVCOMStructure::ChunkInfo chunk1 = structure.getChunkForNode(385, 0, 0);
	FVCOMStructure::ChunkInfo chunk2 = structure.getChunkForNode(385, 0, 80);
	FVCOMStructure::ChunkInfo chunk3 = structure.getChunkForNode(385, 54, 80);

	FVCOMStructure::ChunkInfo chunk4 = structure.getChunkForNode(0, 0, 0);
	FVCOMStructure::ChunkInfo chunk5 = structure.getChunkForNode(4, 0, 0);

	ASSERT_EQ(9990, chunk1.id);
	ASSERT_EQ(5, chunk1.xChunk);
	ASSERT_EQ(11, chunk1.yChunk);
	ASSERT_EQ(0, chunk1.siglayChunk);
	ASSERT_EQ(0, chunk1.timeChunk);

	ASSERT_EQ(9998, chunk2.id);
	ASSERT_EQ(5, chunk2.xChunk);
	ASSERT_EQ(11, chunk2.yChunk);
	ASSERT_EQ(0, chunk2.siglayChunk);
	ASSERT_EQ(8, chunk2.timeChunk);
	

	ASSERT_EQ(10043, chunk3.id);
	ASSERT_EQ(5, chunk3.xChunk);
	ASSERT_EQ(11, chunk3.yChunk);
	ASSERT_EQ(5, chunk3.siglayChunk);
	ASSERT_EQ(8, chunk3.timeChunk);

	ASSERT_EQ(0, chunk4.id);
	ASSERT_EQ(0, chunk4.xChunk);
	ASSERT_EQ(0, chunk4.yChunk);
	ASSERT_EQ(0, chunk4.siglayChunk);
	ASSERT_EQ(0, chunk4.timeChunk);

	ASSERT_EQ(34200, chunk5.id);
	ASSERT_EQ(19, chunk5.xChunk);
	ASSERT_EQ(0, chunk5.yChunk);
	ASSERT_EQ(0, chunk5.siglayChunk);
	ASSERT_EQ(0, chunk5.timeChunk);
}



TEST(FVCOMStructureTest, GetChunkForTriangle) {
	//model extent
	////x: -100 , 100
	////y: -100 , 100
	////siglay: 99
	////time: 81

	////Triangle: 1
	//////X,Y 1: -53.81198, 10

	//x Chunk: 4
	//y Chunk: 11
	//siglay: 0
	//time: 0
	//CHUNK: 8190

	//siglay: 0
	//time: 80
	//CHUNK: 8198

	//siglay: 54
	//time: 80
	//CHUNK: 8243

	FVCOMStructure::ChunkInfo chunk1 = structure.getChunkForTriangle(1, 0, 0);
	FVCOMStructure::ChunkInfo chunk2 = structure.getChunkForTriangle(1, 0, 80);
	FVCOMStructure::ChunkInfo chunk3 = structure.getChunkForTriangle(1, 54, 80);


	ASSERT_EQ(8190, chunk1.id);
	ASSERT_EQ(4, chunk1.xChunk);
	ASSERT_EQ(11, chunk1.yChunk);
	ASSERT_EQ(0, chunk1.siglayChunk);
	ASSERT_EQ(0, chunk1.timeChunk);

	ASSERT_EQ(8198, chunk2.id);
	ASSERT_EQ(4, chunk2.xChunk);
	ASSERT_EQ(11, chunk2.yChunk);
	ASSERT_EQ(0, chunk2.siglayChunk);
	ASSERT_EQ(8, chunk2.timeChunk);

	ASSERT_EQ(8243, chunk3.id);
	ASSERT_EQ(4, chunk3.xChunk);
	ASSERT_EQ(11, chunk3.yChunk);
	ASSERT_EQ(5, chunk3.siglayChunk);
	ASSERT_EQ(8, chunk3.timeChunk);

}

TEST(FVCOMStructureTest, PointInModel) {
	FVCOMStructure::Point inside;
	FVCOMStructure::Point positionOutside1;
	FVCOMStructure::Point depthOutside1;
	FVCOMStructure::Point positionOutside2;
	FVCOMStructure::Point depthOutside2;

	inside.x = 0;
	inside.y = 0;
	inside.h = 0;

	positionOutside1.x = 101;
	positionOutside1.y = 0;
	positionOutside1.h = 0;

	positionOutside2.x = -101;
	positionOutside2.y = 0;
	positionOutside2.h = 0;

	depthOutside1.x = 0;
	depthOutside1.y = 0;
	depthOutside1.h = -301;

	depthOutside2.x = 0;
	depthOutside2.y = 0;
	depthOutside2.h = 1;

	ASSERT_TRUE(structure.pointInModel(inside, 0));

	ASSERT_FALSE(structure.pointInModel(inside, -1));
	ASSERT_FALSE(structure.pointInModel(inside, 1));

	ASSERT_FALSE(structure.pointInModel(positionOutside1, 0));
	ASSERT_FALSE(structure.pointInModel(positionOutside2, 0));
	
	ASSERT_FALSE(structure.pointInModel(depthOutside1, 0));
	ASSERT_FALSE(structure.pointInModel(depthOutside2, 0));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
