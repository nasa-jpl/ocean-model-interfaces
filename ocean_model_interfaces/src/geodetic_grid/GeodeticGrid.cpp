#include "ocean_model_interfaces/geodetic_grid/GeodeticGrid.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"
#include "ocean_model_interfaces/util/Point.h"

#include <stdexcept>
#include <math.h>
#include  <limits>

using namespace ocean_model_interfaces;

GeodeticGrid::GeodeticGrid() {}

GeodeticGrid::GeodeticGrid(GeodeticGridParameters parameters) : structure(GeodeticGridStructure(parameters)),
                                                                parameters(parameters){
    chunkCache = LRUCache<unsigned int, GeodeticGridChunk>(parameters.cacheSize);
}

void GeodeticGrid::setLoadFunction(std::function<void(void)> startLoad, std::function<void(void)> endLoad) {
    this->startLoad = startLoad;
    this->endLoad = endLoad;
}

const ModelData GeodeticGrid::getDataAtIndex(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    if(!structure.indexInRange(timeIndex, depthIndex, latIndex, lonIndex)) {
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
    modelData.depth = structure.indexWaterColumnDepth(latIndex, lonIndex);

    return modelData;
}

const ModelData GeodeticGrid::getDataHelper(double x, double y, double z, double time) {
    if(!structure.timeInModel(time) || !structure.depthInModel(Point(x,y,z)) || !structure.xyInModel(Point(x,y,z))) {
        throw std::out_of_range("GeodeticGrid request outside of model extent");
    }

    ModelData data;
    data.u = 0;
    data.v = 0;
    data.w = 0;
    data.salt = 0;
    data.temp = 0;
    data.dye = 0;
    data.depth = structure.interpolateWaterColumnDepth(Point(x,y,z));
    auto weights = structure.getDataInterpolationWeights(Point(x,y,z), time);

    for (auto weight : weights) {
        ModelData indexData = getDataAtIndex(std::get<0>(weight.first), std::get<1>(weight.first), std::get<2>(weight.first), std::get<3>(weight.first));
        data.u += indexData.u * weight.second;
        data.v += indexData.v * weight.second;
        data.w += indexData.w * weight.second;
        data.salt += indexData.salt * weight.second;
        data.temp += indexData.temp * weight.second;
        data.dye += indexData.dye * weight.second;

       // std::cout << "WEIGHT/INDEX: " << weight.second << " " <<  std::get<0>(weight.first) << " " << std::get<1>(weight.first) << " " << std::get<2>(weight.first) << " " << std::get<3>(weight.first) << std::endl;
      //  std::cout << "VALUE: " << indexData.dye << std::endl;
    }

    return data;
}

const ModelData GeodeticGrid::getDataOutOfRangeHelper(double x, double y, double z, double time) {
    ModelData data;
    data.u = std::numeric_limits<double>::quiet_NaN();
    data.v = std::numeric_limits<double>::quiet_NaN();
    data.w = std::numeric_limits<double>::quiet_NaN();
    data.salt = std::numeric_limits<double>::quiet_NaN();
    data.temp = std::numeric_limits<double>::quiet_NaN();
    data.dye = std::numeric_limits<double>::quiet_NaN();
    data.depth = std::numeric_limits<double>::quiet_NaN();

    Point point;
    point.x = x;
    point.y = y;
    point.z = z;

    if(structure.xyInModel(point))
    {
        data.depth = structure.interpolateWaterColumnDepth(point);
    }

    return data;
}
