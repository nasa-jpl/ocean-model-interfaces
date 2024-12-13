#include "ocean_model_interfaces/geodetic_grid/GeodeticGridChunk.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

#include <stdexcept>
#include <math.h>
#include  <limits>

using namespace ocean_model_interfaces;

GeodeticGridChunk::GeodeticGridChunk(GeodeticGridStructure::ChunkInfo info, std::vector<GeodeticGridStructure::ModelFile> modelFiles) : info(info) {
    std::vector<std::string> dataFields = ["u", "v", "w", "salt", "temp", "dye_01"];
    //TODO: Load model data

    unsigned int currentTimeIndexLoading = info.timeStart;
    unsigned int remainingTimeDimToLoad = info.timeSize;
    for(int i = 0; i < modelFiles.size(); i++) {
        unsigned int timeDimToLoad = std::min(remainingTimeDimToLoad, modelFiles[i].timeDim);

        
        //Load each data field from each file that is needed based on the start indicies and counts
    }
}

ModelData GeodeticGridChunk::getData(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    ModelData data;

    data.u = dataFields["u"].index({timeIndex - info.timeStart, 
                                    depthIndex - info.depthStart
                                    latIndex - info.latStart
                                    lonIndex - info.lonStart});

    data.v = dataFields["v"].index({timeIndex - info.timeStart, 
                                    depthIndex - info.depthStart
                                    latIndex - info.latStart
                                    lonIndex - info.lonStart});

    data.w = dataFields["w"].index({timeIndex - info.timeStart, 
                                    depthIndex - info.depthStart
                                    latIndex - info.latStart
                                    lonIndex - info.lonStart});

    data.temp = dataFields["temp"].index({timeIndex - info.timeStart, 
                                          depthIndex - info.depthStart
                                          latIndex - info.latStart
                                          lonIndex - info.lonStart});

    data.salt = dataFields["salt"].index({timeIndex - info.timeStart, 
                                          depthIndex - info.depthStart
                                          latIndex - info.latStart
                                          lonIndex - info.lonStart});

    if (dataFields.contains("dye_01")) {
        data.dye = dataFields["dye_01"].index({timeIndex - info.timeStart, 
                                            depthIndex - info.depthStart
                                            latIndex - info.latStart
                                            lonIndex - info.lonStart});
    } else {
        data.dye = std::numeric_limits<double>::quiet_NaN();
    }

    //Water column depth isn't included in the chunks so just set that to NaN for now and fill it in later.
    data.depth = std::numeric_limits<double>::quiet_NaN();

    return data;
}
