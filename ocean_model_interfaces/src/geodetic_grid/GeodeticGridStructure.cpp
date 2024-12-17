#include "ocean_model_interfaces/geodetic_grid/GeodeticGridStructure.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"
#include "ocean_model_interfaces/util/UtilityFunctions.h"

#include <stdexcept>
#include <math.h>
#include  <limits>
#include <iostream>

using namespace ocean_model_interfaces;

GeodeticGridStructure::GeodeticGridStructure(GeodeticGridParameters parameters) {
    this->parameters = parameters;
    loadStructureData();
    determineChunksPerDimension();
}

void GeodeticGridStructure::loadStructureData() {
    loadTime();

    netCDF::NcFile singleDataFile(modelFiles[0].filename, netCDF::NcFile::read);
    //Get dimensions of structure elements
    unsigned int latDim = singleDataFile.getDim("eta_rho").getSize();
    unsigned int lonDim = singleDataFile.getDim("xi_rho").getSize();
    unsigned int depthDim = singleDataFile.getDim("s_rho").getSize();

    netCDF::NcVar latVar = singleDataFile.getVar("lat_rho");
    netCDF::NcVar lonVar = singleDataFile.getVar("lon_rho");
    netCDF::NcVar depthVar = singleDataFile.getVar("z_rho0");
    netCDF::NcVar waterColumnDepthVar = singleDataFile.getVar("h");

    latitudes.resize(latDim);
    longitudes.resize(lonDim);


    //Sanity check that this is indeed a north aligned grid
    std::vector<double> latGridCheck;
    std::vector<double> lonGridCheck;
    std::vector<size_t> latGridCheckStart = {0, 0};
    std::vector<size_t> latGridCheckCount = {1, lonDim};
    std::vector<size_t> lonGridCheckStart = {0, 0};
    std::vector<size_t> lonGridCheckCount = {latDim, 1};

    latGridCheck.resize(lonDim);
    lonGridCheck.resize(latDim);
    latVar.getVar(latGridCheckStart, latGridCheckCount, latGridCheck.data());
    lonVar.getVar(lonGridCheckStart, lonGridCheckCount, lonGridCheck.data());

    for(int i = 0; i < latGridCheck.size(); i++) {
        if(latGridCheck[i] != latGridCheck[0]) {
             throw std::runtime_error("Provided latitude grid is not north aligned");
        }
    }

    for(int i = 0; i < lonGridCheck.size(); i++) {
        if(lonGridCheck[i] != lonGridCheck[0]) {
             throw std::runtime_error("Provided longitude grid is not north aligned");
        }
    }

    //Load the actual lat and lon values now
    std::vector<size_t> latStart = {0, 0};
    std::vector<size_t> latCount = {latDim, 1};
    latVar.getVar(latStart, latCount, latitudes.data());

    std::vector<size_t> lonStart = {0, 0};
    std::vector<size_t> lonCount = {1, lonDim};
    lonVar.getVar(lonStart, lonCount, longitudes.data());

    depths = MultiDimensionalVector<double>({depthDim, latDim, lonDim});
    depthVar.getVar(depths.getDataArray());

    waterColumnDepth = MultiDimensionalVector<double>({latDim, lonDim});
    waterColumnDepthVar.getVar(waterColumnDepth.getDataArray());
}

void GeodeticGridStructure::loadTime() {
    unsigned int timeDim = 0;
    std::vector<std::string> filenames = traverseDataFiles(parameters.modelDirectory);

    //Set start times and time dimensions from files
    for(auto &filename : filenames)
    {
        netCDF::NcFile dataFile(filename, netCDF::NcFile::read);
        timeDim += dataFile.getDim("ocean_time").getSize();
        
        std::vector<float> tempTimes;
        tempTimes.resize(dataFile.getDim("ocean_time").getSize());
        netCDF::NcVar timeVar = dataFile.getVar("ocean_time");
        timeVar.getVar(tempTimes.data());
        ModelFile modelFile;
        modelFile.filename = filename;
        modelFile.startTime = tempTimes[0];
        modelFile.timeDim = dataFile.getDim("ocean_time").getSize();
        modelFiles.push_back(modelFile);

        if(modelFile.timeDim > 1) {
            std::cerr << "The GeodeticGrid model interface has only been tested with model files which each hold a single time step. "
                      << "The provided model files have " << modelFile.timeDim << "timesteps in a single file which is untested."
                      << "The interface was implement to support this, but it is recommended that this functionality is tested before using." << std::endl;
        }
    }

    //sort filenames based on start ties
    std::sort(modelFiles.begin(), modelFiles.end());

    //load time variables into one vector
    times.resize(timeDim);
    unsigned int currentIndex = 0;
    for(auto &modelFile : modelFiles)
    {
        netCDF::NcFile dataFile(modelFile.filename, netCDF::NcFile::read);
        netCDF::NcVar timeVar = dataFile.getVar("time");

        //Set the start time index for this file
        modelFile.startTimeIndex = currentIndex;

        //Load times from this file
        timeVar.getVar(times.data() + currentIndex);

        //move current index for next files
        currentIndex += dataFile.getDim("time").getSize();
    }
}

void GeodeticGridStructure::determineChunksPerDimension() {
    timeDimChunks = std::ceil((times.size() / parameters.timeChunkSize));
    depthDimChunks = std::ceil((depths.size()[0] / parameters.depthChunkSize));
    latDimChunks = std::ceil((latitudes.size() / parameters.latChunkSize));
    lonDimChunks = std::ceil((longitudes.size() / parameters.lonChunkSize));
}

GeodeticGridStructure::ChunkInfo GeodeticGridStructure::getGridChunkInfo(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    GeodeticGridStructure::ChunkInfo info;
    info.id = getChunkIdFromIndicies(timeIndex, depthIndex, latIndex, lonIndex);
    info.timeChunk = timeIndex / parameters.timeChunkSize;
    info.depthChunk = depthIndex / parameters.depthChunkSize;
    info.latChunk = latIndex / parameters.latChunkSize;
    info.lonChunk = lonIndex / parameters.lonChunkSize;

    info.timeStart = info.timeChunk * parameters.timeChunkSize;
    info.depthStart = info.depthChunk * parameters.depthChunkSize;
    info.latStart = info.latChunk * parameters.latChunkSize;
    info.lonStart = info.lonChunk * parameters.lonChunkSize;

    info.timeSize = std::min<unsigned int>(parameters.timeChunkSize, times.size() - info.timeStart);
    info.depthSize = std::min<unsigned int>(parameters.depthChunkSize, depths.size()[0] - info.depthStart);
    info.latSize = std::min<unsigned int>(parameters.latChunkSize, latitudes.size() - info.latStart);
    info.lonSize = std::min<unsigned int>(parameters.lonChunkSize, longitudes.size() - info.lonStart);

    return info;
}

unsigned int GeodeticGridStructure::getChunkIdFromIndicies(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    unsigned int timeChunk = timeIndex / parameters.timeChunkSize;
    unsigned int depthChunk = depthIndex / parameters.depthChunkSize;
    unsigned int latChunk = latIndex / parameters.latChunkSize;
    unsigned int lonChunk = lonIndex / parameters.lonChunkSize;

    return lonChunk + (latChunk * lonDimChunks) + (depthChunk * latDimChunks * lonDimChunks) + (timeChunk * depthDimChunks * latDimChunks * lonDimChunks);
}

bool GeodeticGridStructure::isInRange(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    if(timeIndex >= times.size() ||
       depthIndex >= depths.size()[0] ||
       latIndex >= latitudes.size() ||
       lonIndex >= longitudes.size()) {
        return false;
    }
    return true;
}

double GeodeticGridStructure::getWaterColumnDepth(unsigned int latIndex, unsigned int lonIndex) {
    return waterColumnDepth.index({latIndex, lonIndex});
}

std::vector<GeodeticGridStructure::ModelFile>& GeodeticGridStructure::getModelFiles() {
    return modelFiles;
}
