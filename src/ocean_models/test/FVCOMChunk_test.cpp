#include "ocean_models/fvcom/FVCOMStructure.h"
#include "ocean_models/fvcom/FVCOMChunk.h"
#include <gtest/gtest.h>

using namespace ocean_models;

const FVCOMStructure structure("../../src/ocean_models/test_data/box_plume_split", 50, 50, 10, 10);


TEST(FCVOMChunkTest, GetNodeData) {
	FVCOMStructure::ChunkInfo chunkInfo = structure.getChunkForNode(1,0,0);

	const std::vector<unsigned int>& nodes = structure.getNodesInChunk(chunkInfo);
	const std::vector<unsigned int>& triangles = structure.getTrianglesInChunk(chunkInfo);

	FVCOMChunk chunk(structure.getModelFiles(), nodes, triangles, chunkInfo);
	const FVCOMChunk::NodeData& data1 = chunk.getNodeData(1, 0, 0);
	const FVCOMChunk::NodeData& data2 = chunk.getNodeData(1, 9, 0);
	const FVCOMChunk::NodeData& data3 = chunk.getNodeData(1, 9, 9);

	const FVCOMChunk::NodeData& data4 = chunk.getNodeData(17, 0, 0);
	const FVCOMChunk::NodeData& data5 = chunk.getNodeData(17, 9, 0);
	const FVCOMChunk::NodeData& data6 = chunk.getNodeData(17, 9, 9);

	ASSERT_FLOAT_EQ(3.2964647, data1.temp);
	ASSERT_FLOAT_EQ(34.71025, data1.salt);

	ASSERT_FLOAT_EQ(3.2328281, data2.temp);
	ASSERT_FLOAT_EQ(34.714798, data2.salt);

	ASSERT_FLOAT_EQ(3.2328043, data3.temp);
	ASSERT_FLOAT_EQ(34.714607, data3.salt);

	ASSERT_FLOAT_EQ(3.2964647, data4.temp);
	ASSERT_FLOAT_EQ(34.71025, data4.salt);

	ASSERT_FLOAT_EQ(3.2328281, data5.temp);
	ASSERT_FLOAT_EQ(34.714798, data5.salt);

	ASSERT_FLOAT_EQ(3.2328043, data6.temp);
	ASSERT_FLOAT_EQ(34.714607, data6.salt);
}


TEST(FCVOMChunkTest, GetTriangleData) {
	FVCOMStructure::ChunkInfo chunkInfo = structure.getChunkForNode(1,0,0);
	const std::vector<unsigned int>& nodes = structure.getNodesInChunk(chunkInfo);
	const std::vector<unsigned int>& triangles = structure.getTrianglesInChunk(chunkInfo);

	FVCOMChunk chunk(structure.getModelFiles(), nodes, triangles, chunkInfo);
	const FVCOMChunk::TriangleData& data1 = chunk.getTriangleData(51, 0, 0);
	const FVCOMChunk::TriangleData& data2 = chunk.getTriangleData(51, 9, 0);
	const FVCOMChunk::TriangleData& data3 = chunk.getTriangleData(51, 9, 9);

	const FVCOMChunk::TriangleData& data4 = chunk.getTriangleData(8, 0, 0);
	const FVCOMChunk::TriangleData& data5 = chunk.getTriangleData(8, 9, 0);
	const FVCOMChunk::TriangleData& data6 = chunk.getTriangleData(8, 9, 9);

	ASSERT_FLOAT_EQ(0.0, data1.u);
	ASSERT_FLOAT_EQ(0.0, data1.v);
	ASSERT_FLOAT_EQ(0.0, data1.w);

	ASSERT_FLOAT_EQ(0.0, data2.u);
	ASSERT_FLOAT_EQ(0.0, data2.v);
	ASSERT_FLOAT_EQ(0.0, data2.w);

	ASSERT_FLOAT_EQ(0.000010166797, data3.u);
	ASSERT_FLOAT_EQ(-0.000012173005, data3.v);
	ASSERT_FLOAT_EQ(0.00013492772, data3.w);

	ASSERT_FLOAT_EQ(0.0, data4.u);
	ASSERT_FLOAT_EQ(0.0, data4.v);
	ASSERT_FLOAT_EQ(0.0, data4.w);

	ASSERT_FLOAT_EQ(0.0, data5.u);
	ASSERT_FLOAT_EQ(0.0, data5.v);
	ASSERT_FLOAT_EQ(0.0, data5.w);

	ASSERT_FLOAT_EQ(0.000052855106, data6.u);
	ASSERT_FLOAT_EQ(0.00016515434, data6.v);
	ASSERT_FLOAT_EQ(-0.000160249, data6.w);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
