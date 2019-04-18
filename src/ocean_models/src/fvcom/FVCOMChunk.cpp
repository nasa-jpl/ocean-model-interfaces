#include "fvcom/FVCOMChunk.h"
#include "fvcom/FVCOMStructure.h"

#include <unordered_map>
#include <vector>

#include <netcdf>

FVCOMChunk::FVCOMChunk(const std::vector<FVCOMStructure::ModelFile> modelFiles, std::vector<unsigned int> nodesToLoad,
											   std::vector<unsigned int> trianglesToLoad,
											   FVCOMStructure::ChunkInfo chunkInfo) :
	chunkInfo(chunkInfo)
{
	unsigned int startModelFile = getFileIndexForTimeIndex(modelFiles, chunkInfo.timeStart);
	unsigned int endModelFile = getFileIndexForTimeIndex(modelFiles, chunkInfo.timeStart + chunkInfo.timeSize);
	
	nodes.reserve(nodesToLoad.size());
	triangles.reserve(trianglesToLoad.size());

	std::vector<float> uLoad;
	std::vector<float> vLoad;
	std::vector<float> wLoad;
	std::vector<float> tempLoad;
	std::vector<float> saltLoad;	
	std::vector<float> dyeLoad;

	//Initalize node data storage
	for(unsigned int i = 0; i < nodesToLoad.size(); i++)
	{
		tempLoad.resize(chunkInfo.timeSize * chunkInfo.siglaySize);
		saltLoad.resize(chunkInfo.timeSize * chunkInfo.siglaySize);
		dyeLoad.resize(chunkInfo.timeSize * chunkInfo.siglaySize);

		nodes.insert(std::make_pair(nodesToLoad[i], std::vector<FVCOMChunk::NodeData>(tempLoad.size())));
	}

	//Initalize triange data storage
	for(unsigned int i = 0; i < trianglesToLoad.size(); i++)
	{
		uLoad.resize(chunkInfo.timeSize * chunkInfo.siglaySize);
		vLoad.resize(chunkInfo.timeSize * chunkInfo.siglaySize);
		wLoad.resize(chunkInfo.timeSize * chunkInfo.siglaySize);

		triangles.insert(std::make_pair(trianglesToLoad[i], std::vector<FVCOMChunk::TriangleData>(uLoad.size())));
	}
	bool dyeVarExists = true;
	for(unsigned int i = 0; i < nodesToLoad.size(); i++)
	{
		unsigned int timeIndex = chunkInfo.timeStart;
		unsigned int dataIndex = 0;

		for(unsigned int f = startModelFile; f <= endModelFile; f++)
		{
			netCDF::NcFile dataFile(modelFiles[f].filename, netCDF::NcFile::read);
			netCDF::NcVar tempVar = dataFile.getVar("temp");
			netCDF::NcVar saltVar = dataFile.getVar("salinity");
			netCDF::NcVar dyeVar = dataFile.getVar("DYE");

			//Adjust time index for this file 
			unsigned int adjustedTimeIndex = timeIndex - modelFiles[f].startTimeIndex;

			//calculate the size of the time dimension that needs to be loaded
			unsigned int timeCount = std::min(chunkInfo.timeSize - (timeIndex - chunkInfo.timeStart), modelFiles[f].timeDim - adjustedTimeIndex);

			//set start and count variables
			std::vector<size_t> start = {adjustedTimeIndex, chunkInfo.siglayStart, nodesToLoad[i]};
			std::vector<size_t> count = {timeCount, chunkInfo.siglaySize, 1};


			//load data
			tempVar.getVar(start, count, tempLoad.data() + dataIndex);
			saltVar.getVar(start, count, saltLoad.data() + dataIndex);
			//Load the dye variable if it exists
			if(!dyeVar.isNull())
			{
				dyeVar.getVar(start, count, dyeLoad.data() + dataIndex);
			}
			else
			{
				dyeVarExists = false;
			}
			

			//Update time and data indicies
			timeIndex += timeCount;
			dataIndex += timeCount * chunkInfo.siglaySize;
		}

		std::vector<FVCOMChunk::NodeData>& dataList = nodes[nodesToLoad[i]];

		//populate vector of NodeData objects
		for(unsigned int j = 0; j < tempLoad.size(); j++)
		{
			FVCOMChunk::NodeData data;
			data.temp = tempLoad[j];
			data.salt = saltLoad[j];

			if(dyeVarExists)
			{
				data.dye = dyeLoad[j];
			}
			else
			{
				data.dye = 0;
			}
			dataList[j] = data;
		}
	}
	for(unsigned int i = 0; i < trianglesToLoad.size(); i++)
	{
		unsigned int timeIndex = chunkInfo.timeStart;
		unsigned int dataIndex = 0;

		for(unsigned int f = startModelFile; f <= endModelFile; f++)
		{
			netCDF::NcFile dataFile(modelFiles[f].filename, netCDF::NcFile::read);
			netCDF::NcVar uVar = dataFile.getVar("u");
			netCDF::NcVar vVar = dataFile.getVar("v");
			netCDF::NcVar wVar = dataFile.getVar("ww");

			//Adjust time index for this file 
			unsigned int adjustedTimeIndex = timeIndex - modelFiles[f].startTimeIndex;

			//calculate the size of the time dimension that needs to be loaded
			unsigned int timeCount = std::min(chunkInfo.timeSize - (timeIndex - chunkInfo.timeStart), modelFiles[f].timeDim - adjustedTimeIndex);

			//set start and count variables
			std::vector<size_t> start = {adjustedTimeIndex, chunkInfo.siglayStart, trianglesToLoad[i]};
			std::vector<size_t> count = {timeCount, chunkInfo.siglaySize, 1};

			uVar.getVar(start, count, uLoad.data() + dataIndex);
			vVar.getVar(start, count, vLoad.data() + dataIndex);
			wVar.getVar(start, count, wLoad.data() + dataIndex);

			//Update time and data indicies
			timeIndex += timeCount;
			dataIndex += timeCount * chunkInfo.siglaySize;
		}

		std::vector<FVCOMChunk::TriangleData>& dataList = triangles[trianglesToLoad[i]];

		//populate vector of TriangleData objects
		for(unsigned int j = 0 ; j < uLoad.size(); j++)
		{
			FVCOMChunk::TriangleData data;
			data.u = uLoad[j];
			data.v = vLoad[j];
			data.w = wLoad[j];
			dataList[j] = data;
		}
	}
	
}

const unsigned int FVCOMChunk::getFileIndexForTimeIndex(const std::vector<FVCOMStructure::ModelFile> modelFiles, const unsigned int timeIndex) const
{
	for(int i = modelFiles.size() - 1; i >= 0; i--)
	{
		if(modelFiles[i].startTimeIndex <= timeIndex)
		{
			return i;
		}
	}
	return modelFiles.size();
}

const FVCOMChunk::NodeData& FVCOMChunk::getNodeData(const unsigned int node, const unsigned int siglay, const unsigned int time)
{
	std::vector<FVCOMChunk::NodeData>& data = nodes[node];
	unsigned int index = (siglay - chunkInfo.siglayStart) + (time - chunkInfo.timeStart) * chunkInfo.siglaySize;
	return data[index];
}

const FVCOMChunk::TriangleData& FVCOMChunk::getTriangleData(const unsigned int triangle, const unsigned int siglay, const unsigned int time)
{
	const std::vector<FVCOMChunk::TriangleData>& data = triangles[triangle];
	unsigned int index = (siglay - chunkInfo.siglayStart) + (time - chunkInfo.timeStart) * chunkInfo.siglaySize;

	return data[index];
}
