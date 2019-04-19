#ifndef FVCOM_H
#define FVCOM_H

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <netcdf>
#include <exception>

#include "fvcom/FVCOMStructure.h"
#include "fvcom/FVCOMChunk.h"
#include "fvcom/LRUCache.h"

#include "model_interface/ModelInterface.h"
#include "model_interface/ModelData.h"

namespace ocean_models
{

/**
 * Class used to load and query FVCOM data
 */
class FVCOM : public ModelInterface
{
public:

	/**
	 * Initalize FVCOM class with no data file,
     */
	FVCOM();

	/**
	 * Initalize FVCOM class with data from file,
     * @param filename File to load
     */
	FVCOM(std::string filename);

	/**
	 * Initalize FVCOM class with data from file,
     * @param filename File to load
     * @param start_load function to call before new data is loaded
     * @param end_load function to call after new data is loaded
     */
	FVCOM(std::string filename, void (*startLoad)(void), void (*endLoad)(void));

	/**
	 * Initalize FVCOM class with data from file and specify the size of the FVCOMChunks
     * @param filename File to load
     * @param xChunkSize Size of a chunk in the x direction
     * @param yChunkSize Size of a chunk in the y direction
     * @param siglayChunkSize Size of a chunk in the siglay direction
     * @param timeChunkSize Size of a chunk in the time direction
     */
	FVCOM(std::string filename, unsigned int xChunkSize, 
								unsigned int yChunkSize,
								unsigned int siglayChunkSize, 
								unsigned int timeChunkSize, 
								unsigned int cacheSize);

	/**
	 * Initalize FVCOM class with data from file and specify the size of the FVCOMChunks
     * @param filename File to load
     * @param xChunkSize Size of a chunk in the x direction
     * @param yChunkSize Size of a chunk in the y direction
     * @param siglayChunkSize Size of a chunk in the siglay direction
     * @param timeChunkSize Size of a chunk in the time direction
     * @param start_load function to call before new data is loaded
     * @param end_load function to call after new data is loaded
     */
	FVCOM(std::string filename,
		  void (*startLoad)(void),
		  void (*endLoad)(void),
		  unsigned int xChunkSize,
		  unsigned int yChunkSize,
		  unsigned int siglayChunkSize,
		  unsigned int timeChunkSize,
		  unsigned int cacheSize);

	const ModelData getData(double x, double y, double height, double time);
	const ModelData getDataOutOfRange(double x, double y, double height, double time);

private:

	/**
	 * Retrieves data from a specific node
	 * If the needed data is not loaded this function will call all the necessary functions to load it 
	 * @param node Node number of get data from
	 * @param sigma Sigma layer to get the data from
	 * @param time Time index to get the data from
	 */
	const FVCOMChunk::NodeData& getNodeData(int node, int siglayNodeIndex, int timeIndex);

	/**
	 * Retrieves data from a specific triangle
	 * If the needed data is not loaded this function will call all the necessary functions to load it 
	 * @param triangle Triangle number of get data from
	 * @param sigma Sigma layer to get the data from
	 * @param time Time index to get the data from
	 */
	const FVCOMChunk::TriangleData& getTriangleData(int triangle, int siglayTriangleIndex, int timeIndex);

	ModelData interpolate(FVCOMStructure::Point p, double time);
	FVCOMChunk::NodeDataInterp nodeInterpolation(const FVCOMStructure::Point& interpolatedPoint, int containingTriangle, int siglayIndex, int timeIndex);
	FVCOMChunk::TriangleDataInterp triangleInterpolation(const FVCOMStructure::Point& interpolatedPoint, int containingTriangle, int siglayIndex, int timeIndex);
	const double areaOfTriangle(const FVCOMStructure::Point& p1, const FVCOMStructure::Point& p2, const FVCOMStructure::Point& p3) const;

private:
	
	FVCOMStructure structure;

	LRUCache<unsigned int, FVCOMChunk> chunkCache;

	void (*startLoad)(void);
	void (*endLoad)(void);

};

}
#endif
