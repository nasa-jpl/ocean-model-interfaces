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
#include "ocean_model_interfaces/geodetic_grid/GeodeticGridChunk.h"
#include "ocean_model_interfaces/util/LRUCache.h"

#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

namespace ocean_model_interfaces
{

struct GeodeticGridParameters {
    //The directory that the model is contained in.
    std::string modelDirectory;

    //The size of the chunks to load in number of indicies
    int timeChunkSize = 1;
    int depthChunkSize = 40;
    int latChunkSize = 10;
    int lonChunkSize = 10;
}

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

private:

    ModelData indexModelData(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);

private:
    LRUCache<unsigned int, GeodeticGridChunk> chunkCache;
    GeodeticGridStructure structure
    GeodeticGridParameters parameters;
    std::function<void(void)> startLoad;
    std::function<void(void)> endLoad;

};

}
#endif
