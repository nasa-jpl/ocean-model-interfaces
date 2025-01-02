#include "ocean_model_interfaces/geodetic_grid/GeodeticGridChunk.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

#include <stdexcept>
#include <math.h>
#include  <limits>

using namespace ocean_model_interfaces;

GeodeticGridChunk::GeodeticGridChunk(GeodeticGridStructure::ChunkInfo info, std::vector<GeodeticGridStructure::ModelFile>& modelFiles) : info(info) {
    std::vector<std::string> dataFieldStrings = {"u", "v", "w", "salt", "temp", "dye_01"};

    //Initialize the data fields and sizes
    for(uint i = 0; i < dataFieldStrings.size(); i++) {
        dataFields[dataFieldStrings[i]] = MultiDimensionalVector<double>({info.timeSize, info.depthSize, info.latSize, info.lonSize});
    }

    unsigned int currentTimeIndexLoading = info.timeStart;
    unsigned int remainingTimeDimToLoad = info.timeSize;
    for(int i = 0; i < modelFiles.size(); i++) {
        unsigned int adjustedTimeStart = currentTimeIndexLoading - modelFiles[i].startTimeIndex;

        if(0 <= adjustedTimeStart && adjustedTimeStart < modelFiles[i].timeDim) {
            netCDF::NcFile dataFile(modelFiles[i].filename, netCDF::NcFile::read);

            unsigned int timeDimToLoad = std::min(remainingTimeDimToLoad, modelFiles[i].timeDim);
            std::vector<size_t> start = {adjustedTimeStart, info.depthStart, info.latStart, info.lonStart};
            std::vector<size_t> count = {timeDimToLoad, info.depthSize, info.latSize, info.lonSize};

            //Load data for each of the data fields
            for(uint j = 0; j < dataFieldStrings.size(); j++) {
                netCDF::NcVar var = dataFile.getVar(dataFieldStrings[j]);
                var.getVar(start, count, dataFields[dataFieldStrings[j]].getDataArrayAtIndex({currentTimeIndexLoading - info.timeStart,0,0,0}));
            }

            currentTimeIndexLoading += timeDimToLoad;
            remainingTimeDimToLoad -= timeDimToLoad;
            //Load each data field from each file that is needed based on the start indicies and counts
            if(remainingTimeDimToLoad == 0) {
                break;
            }
        }
    }
}

ModelData GeodeticGridChunk::getData(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    ModelData data;

    data.u = dataFields["u"].index({timeIndex - info.timeStart, 
                                    depthIndex - info.depthStart,
                                    latIndex - info.latStart,
                                    lonIndex - info.lonStart});

    data.v = dataFields["v"].index({timeIndex - info.timeStart, 
                                    depthIndex - info.depthStart,
                                    latIndex - info.latStart,
                                    lonIndex - info.lonStart});

    data.w = dataFields["w"].index({timeIndex - info.timeStart, 
                                    depthIndex - info.depthStart,
                                    latIndex - info.latStart,
                                    lonIndex - info.lonStart});

    data.temp = dataFields["temp"].index({timeIndex - info.timeStart, 
                                          depthIndex - info.depthStart,
                                          latIndex - info.latStart,
                                          lonIndex - info.lonStart});

    data.salt = dataFields["salt"].index({timeIndex - info.timeStart, 
                                          depthIndex - info.depthStart,
                                          latIndex - info.latStart,
                                          lonIndex - info.lonStart});

    if (dataFields.find("dye_01") != dataFields.end()) {
        data.dye = dataFields["dye_01"].index({timeIndex - info.timeStart, 
                                            depthIndex - info.depthStart,
                                            latIndex - info.latStart,
                                            lonIndex - info.lonStart});
    } else {
        data.dye = std::numeric_limits<double>::quiet_NaN();
    }

    //Water column depth isn't included in the chunks so just set that to NaN for now and fill it in later.
    data.depth = std::numeric_limits<double>::quiet_NaN();

    return data;
}
