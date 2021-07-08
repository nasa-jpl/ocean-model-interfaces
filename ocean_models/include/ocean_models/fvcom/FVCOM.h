#ifndef FVCOM_H
#define FVCOM_H

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <netcdf>
#include <exception>
#include <functional>

#include "ocean_models/fvcom/FVCOMStructure.h"
#include "ocean_models/fvcom/FVCOMChunk.h"
#include "ocean_models/fvcom/LRUCache.h"

#include "ocean_models/model_interface/ModelInterface.h"
#include "ocean_models/model_interface/ModelData.h"

namespace ocean_models
{

/**
 * Provides random access to FVCOM ocean model data. To handle the large data volumes,
 * only the model structure (location of triangles, vertical layers, and time slices )
 * is initially loaded into memory. When queried specific sections, or "chunks", of the model
 * are loaded and stored using an LRU cache.
 */
class FVCOM : public ModelInterface
{
public:

    /**
     * Initalize FVCOM class with no data file.
     */
    FVCOM();

    /**
     * Initalize FVCOM class with data from single file or directory.
     * If a directory is provided then it will load all netCDF files in that directory.
     * It is assumed that each netCDF file will include one complete timeslice of the model.
     * @param filename File or directory to load
     */
    FVCOM(std::string filename);

    /**
     * Initalize FVCOM class with data from single file or directory.
     * If a directory is provided then it will load all netCDF files in that directory.
     * It is assumed that each netCDF file will include one complete timeslice of the model.
     * Additionally allows specifies function to be called when data is loaded from permenant storage.
     * @param filename File or directory to load
     * @param start_load function to call before new data is loaded
     * @param end_load function to call after new data is loaded
     */
    FVCOM(std::string filename, std::function<void(void)> startLoad, std::function<void(void)> endLoad);

    /**
     * Initalize FVCOM class with data from single file or directory.
     * If a directory is provided then it will load all netCDF files in that directory.
     * It is assumed that each netCDF file will include one complete timeslice of the model.
     * Additionally this allows you to specify the size of a "chunk", or the smallest discrete
     * portion of the model that is loaded into memory.
     * @param filename File or directory to load
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
     * Initalize FVCOM class with data from single file or directory.
     * If a directory is provided then it will load all netCDF files in that directory.
     * It is assumed that each netCDF file will include one complete timeslice of the model.
     * Additionally this allows you to specify the size of a "chunk", or the smallest discrete
     * portion of the model that is loaded into memory and specifies function to be called when
     * data is loaded from permenant storage.
     * @param filename File or directory to load
     * @param xChunkSize Size of a chunk in the x direction
     * @param yChunkSize Size of a chunk in the y direction
     * @param siglayChunkSize Size of a chunk in the siglay direction
     * @param timeChunkSize Size of a chunk in the time direction
     * @param start_load function to call before new data is loaded
     * @param end_load function to call after new data is loaded
     */
    FVCOM(std::string filename,
          std::function<void(void)> startLoad,
          std::function<void(void)> endLoad,
          unsigned int xChunkSize,
          unsigned int yChunkSize,
          unsigned int siglayChunkSize,
          unsigned int timeChunkSize,
          unsigned int cacheSize);

protected:
    /**
     * Helper function implementation from the ModelInterface class. Retrieves data
     * that is inside the model bounds. The reference frame is dependent on the data stored
     * in the FVCOM model.
     * 
     * @param x The x value to retrieve data at.
     * @param y The y value to retrieve data at.
     * @param h The h or z value to retrieve data at.
     * @param time The time value to retrieve data at. Time is specified based on the loaded model data.
     * 
     * @return The model data for the specified location and time.
     */
    const ModelData getDataHelper(double x, double y, double height, double time) override;

    /**
     * Helper function implementation from the ModelInterface class. Handles requests that are outside
     * the model bounds. The reference frame is dependent on the data stored
     * in the FVCOM model.
     * 
     * @param x The x value to retrieve data at.
     * @param y The y value to retrieve data at.
     * @param h The h or z value to retrieve data at.
     * @param time The time value to retrieve data at. Time is specified based on the loaded model data.
     * 
     * @return The model data for the specified location and time.
     */
    const ModelData getDataOutOfRangeHelper(double x, double y, double height, double time) override;

private:

    /**
     * Retrieves data located at a specific node, siglay, and time.
     * If the required data is not loaded in memory then this function will call all the necessary 
     * functions to load it 
     * @param node Node number of get data from
     * @param siglayNodeIndex Sigma layer to get the data from
     * @param timeIndex Time index to get the data from
     * 
     * @return The data located at the specified node.
     */
    const FVCOMChunk::NodeData& getNodeData(int node, int siglayNodeIndex, int timeIndex);

    /**
     * Retrieves data located at the center of a specific triangle, siglay, and time
     * If the required data is not loaded in memory this function will call all the necessary 
     * functions to load it 
     * @param triangle Triangle number of get data from
     * @param siglayTriangleIndex Sigma layer to get the data from
     * @param timeIndex Time index to get the data from
     * 
     * @return The data located at the specified triangle.
     */
    const FVCOMChunk::TriangleData& getTriangleData(int triangle, int siglayTriangleIndex, int timeIndex);

    /**
     * Retreives the interpolated model data at a specified model point and time
     */
    ModelData interpolate(FVCOMStructure::Point p, double time);

    /**
     * Performs XY barycentric linear interpolation of the model variables stored at the 
     * nodes for a specific location using a fixed index siglay and time.
     */
    FVCOMChunk::NodeDataInterp nodeInterpolation(const FVCOMStructure::Point& interpolatedPoint, int containingTriangle, int siglayIndex, int timeIndex);
    
    /**
     * Performs interpolations of the model variables stored at the triangles
     * for a specific location using a fixed index siglay and time. Interpolation
     * currently just used the data from the containing triangle.
     */
    FVCOMChunk::TriangleDataInterp triangleInterpolation(const FVCOMStructure::Point& interpolatedPoint, int containingTriangle, int siglayIndex, int timeIndex);
    
    /**
     * @return The area of the triangle as specified by the provided 3 points.
     */
    const double areaOfTriangle(const FVCOMStructure::Point& p1, const FVCOMStructure::Point& p2, const FVCOMStructure::Point& p3) const;

private:
    LRUCache<unsigned int, FVCOMChunk> chunkCache;
    FVCOMStructure structure;

    std::function<void(void)> startLoad;
    std::function<void(void)> endLoad;

};

}
#endif
