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

#include "ocean_model_interfaces/model_interface/ModelData.h"


namespace ocean_model_interfaces
{

class GeodeticGridChunk
{
public:
    GeodeticGridChunk(GeodeticGridStructure::ChunkInfo info, std::vector<GeodeticGridStructure::ModelFile> modelFiles);
private:
    ModelData& getData(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);

private:
    GeodeticGridStructure::ChunkInfo info;
    std::unordered_map<std::string, MultiDimensionalVector<double>> dataFields;
};

}
#endif
