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

#include "ocean_model_interfaces/fvcom/FVCOMStructure.h"
#include "ocean_model_interfaces/fvcom/FVCOMChunk.h"
#include "ocean_model_interfaces/fvcom/LRUCache.h"

#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

namespace ocean_model_interfaces
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
     * @param startLoad function to call before new data is loaded
     * @param endLoad function to call after new data is loaded
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
     * @param startLoad function to call before new data is loaded
     * @param endLoad function to call after new data is loaded
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
     * in the FVCOM model. With the standard FVCOM setup of postive H variables and negative siglay 
     * variables, z is expected to be negative here.
     * 
     * @param x The x value to retrieve data at.
     * @param y The y value to retrieve data at.
     * @param z The z value to retrieve data at.
     * @param time The time value to retrieve data at. Time is specified based on the loaded model data.
     * 
     * @return The model data for the specified location and time.
     */
    const ModelData getDataHelper(double x, double y, double z, double time) override;

    /**
     * Helper function implementation from the ModelInterface class. Handles requests that are outside
     * the model bounds. All returned model data is NaN with depths taken from the nearest node, if outside
     * the XY bounds of the model. The reference frame is dependent on the data stored
     * in the FVCOM model. With the standard FVCOM setup of postive H variables and negative siglay 
     * variables, z is expected to be negative here.
     * 
     * @param x The x value to retrieve data at.
     * @param y The y value to retrieve data at.
     * @param z The z value to retrieve data at.
     * @param time The time value to retrieve data at. Time is specified based on the loaded model data.
     * 
     * @return The model data for the specified location and time.
     */
    const ModelData getDataOutOfRangeHelper(double x, double y, double z, double time) override;

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
     * @param interpolatedPoint The point to interpolate at
     * @param time The time to interpolate at
     * 
     * @return The interpolated data.
     */
    ModelData interpolate(FVCOMStructure::Point p, double time);

    /**
     * Performs XY barycentric linear interpolation of the model variables stored at the 
     * nodes for a specific location using a fixed index siglay and time.
     * @param interpolatedPoint The point to interpolate at
     * @param containingTriange The triangle that contains the interpolated point
     * @param siglayIndex The siglay index we want to interpolate at
     * @param timeIndex The time index we want to interpolate at
     * 
     * @return Data interpolated at the given point.
     */
    FVCOMChunk::NodeDataInterp nodeInterpolation(const FVCOMStructure::Point& interpolatedPoint, int containingTriangle, int siglayIndex, int timeIndex);
    
    /**
     * Performs interpolations of the model variables stored at the triangles
     * for a specific location using a fixed index siglay and time. Interpolation
     * currently just used the data from the containing triangle.
     *  @param interpolatedPoint The point to interpolate at
     * @param containingTriange The triangle that contains the interpolated point
     * @param siglayIndex The siglay index we want to interpolate at
     * @param timeIndex The time index we want to interpolate at
     * 
     * @return Data interpolated at the given point.
     */
    FVCOMChunk::TriangleDataInterp triangleInterpolation(const FVCOMStructure::Point& interpolatedPoint, int containingTriangle, int siglayIndex, int timeIndex);
    
    /**
     * Finds the area of a given triangle
     * @param p1 Point 1 of the triangle
     * @param p2 Point 2 of the triangle
     * @param p3 Point 3 of the triangle
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
