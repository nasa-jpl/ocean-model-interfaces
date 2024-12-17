#include "ocean_model_interfaces/geodetic_grid/GeodeticGrid.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

#include <stdexcept>
#include <math.h>
#include  <limits>

using namespace ocean_model_interfaces;


GeodeticGrid::GeodeticGrid(GeodeticGridParameters parameters) : structure(GeodeticGridStructure(parameters)),
                                                                parameters(parameters){
}

void GeodeticGrid::setLoadFunction(std::function<void(void)> startLoad, std::function<void(void)> endLoad) {
    this->startLoad = startLoad;
    this->endLoad = endLoad;
}

const ModelData GeodeticGrid::indexModelData(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    if(!structure.isInRange(timeIndex, depthIndex, latIndex, lonIndex)) {
        throw std::runtime_error("Requested model indicies are out of range.");
    }
    unsigned int chunkId = structure.getChunkIdFromIndicies(timeIndex, depthIndex, latIndex, lonIndex);

    if(!chunkCache.exists(chunkId)) {
        if(startLoad) {
            startLoad();
        }

        GeodeticGridStructure::ChunkInfo info = structure.getGridChunkInfo(timeIndex, depthIndex, latIndex, lonIndex);
        chunkCache.put(info.id, GeodeticGridChunk(info, structure.getModelFiles()));

        if(endLoad) {
            endLoad();
        }
    }

    GeodeticGridChunk& chunk = chunkCache.get(chunkId);
    ModelData modelData = chunk.getData(timeIndex, depthIndex, latIndex, lonIndex);
    modelData.depth = structure.getWaterColumnDepth(latIndex, lonIndex);

    return modelData;
}