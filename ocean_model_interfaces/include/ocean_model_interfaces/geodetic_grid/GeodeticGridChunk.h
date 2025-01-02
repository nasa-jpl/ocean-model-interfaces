#ifndef GEODETIC_GRID_CHUNK_H
#define GEODETIC_GRID_CHUNK_H

#include "ocean_model_interfaces/geodetic_grid/GeodeticGridStructure.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <exception>
#include <functional>




namespace ocean_model_interfaces
{

class GeodeticGridChunk
{
public:
    GeodeticGridChunk(GeodeticGridStructure::ChunkInfo info, std::vector<GeodeticGridStructure::ModelFile>& modelFiles);

public:
    ModelData getData(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);

private:
    GeodeticGridStructure::ChunkInfo info;
    std::unordered_map<std::string, MultiDimensionalVector<double>> dataFields;
};

}
#endif
