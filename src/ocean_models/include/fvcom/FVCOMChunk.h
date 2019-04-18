#ifndef FVCOM_CHUNK_H
#define FVCOM_CHUNK_H

#include <unordered_map>
#include <vector>

#include <netcdf>

#include "fvcom/FVCOMStructure.h"

class FVCOMChunk
{
	
public:
	struct NodeData
	{
		float temp;
		float salt;
		float dye;
	};

	struct TriangleData
	{
		float u;
		float v;
		float w;
	};

	struct NodeDataInterp
	{
		double temp;
		double salt;
		double dye;
	};

	struct TriangleDataInterp
	{
		double u;
		double v;
		double w;
	};

	typedef std::vector<FVCOMChunk::NodeData> NodeVector;
	typedef std::vector<FVCOMChunk::TriangleData> TriangleVector;

	FVCOMChunk(const std::vector<FVCOMStructure::ModelFile> modelFiles, std::vector<unsigned int> nodesToLoad,
											   std::vector<unsigned int> trianglesToLoad,
											   FVCOMStructure::ChunkInfo chunkInfo);



	const FVCOMChunk::NodeData& getNodeData(const unsigned int node, const unsigned int siglay, const unsigned int time);
	const FVCOMChunk::TriangleData& getTriangleData(const unsigned int triangle, const unsigned int siglay, const unsigned int time);

private:
	const unsigned int getFileIndexForTimeIndex(const std::vector<FVCOMStructure::ModelFile> modelFiles, const unsigned int timeIndex) const;

private:
	std::unordered_map<unsigned int, FVCOMChunk::NodeVector> nodes;
	std::unordered_map<unsigned int, FVCOMChunk::TriangleVector> triangles;

	const FVCOMStructure::ChunkInfo chunkInfo;
};

#endif
