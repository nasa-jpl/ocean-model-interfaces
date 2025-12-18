#ifndef GEODETIC_GRID_H
#define GEODETIC_GRID_H

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <netcdf>
#include <exception>
#include <functional>

#include "ocean_model_interfaces/geodetic_grid/GeodeticGridStructure.h"
#include "ocean_model_interfaces/geodetic_grid/GeodeticGridParameters.h"
#include "ocean_model_interfaces/geodetic_grid/GeodeticGridChunk.h"
#include "ocean_model_interfaces/util/LRUCache.h"

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
class GeodeticGrid : public ModelInterface
{
public:

    /**
     * Default constructor just so we don't need to initialize it right away if we don't want to.
     */
    GeodeticGrid();

    /**
     * Initalize GeodeticGrid class with no data file.
     */
    GeodeticGrid(GeodeticGridParameters parameters);

    /**
     * @brief Sets the functions that are called before and after data is loaded
     * 
     * @param startLoad function to call before new data is loaded
     * @param endLoad function to call after new data is loaded
     */
    void setLoadFunction(std::function<void(void)> startLoad, std::function<void(void)> endLoad);

    const ModelData getDataAtIndex(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);

    const ModelData getData(double x, double y, double z, double time) override;
    const ModelData getDataOutOfRange(double x, double y, double z, double time) override;

protected:
    /**
     * @brief Interpolates the model at the given location. Note that x is longitude and y is latitude
     * 
     * @param x Longitude for the request
     * @param y Latitude for the request
     * @param z Depth for the request
     * @param time Time for the request
     * @return const ModelData 
     */
    const ModelData getDataHelper(double x, double y, double z, double time) override;

    /**
     * @brief Interpolates the model at the given location if that location is outside the model. Note that x is longitude and y is latitude
     * 
     * @param x Longitude for the request
     * @param y Latitude for the request
     * @param z Depth for the request
     * @param time Time for the request
     * @return const ModelData 
     */
    const ModelData getDataOutOfRangeHelper(double x, double y, double z, double time) override;

private:
    LRUCache<unsigned int, GeodeticGridChunk> chunkCache;
    GeodeticGridStructure structure;
    GeodeticGridParameters parameters;
};

}
#endif
