#include "ocean_model_interfaces/geodetic_grid/GeodeticGridStructure.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"
#include "ocean_model_interfaces/util/UtilityFunctions.h"

#include <stdexcept>
#include <math.h>
#include  <limits>
#include <iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using namespace ocean_model_interfaces;

GeodeticGridStructure::GeodeticGridStructure() {}

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
        netCDF::NcVar timeVar = dataFile.getVar("ocean_time");

        //Set the start time index for this file
        modelFile.startTimeIndex = currentIndex;

        //Load times from this file
        timeVar.getVar(times.data() + currentIndex);

        //move current index for next files
        currentIndex += dataFile.getDim("ocean_time").getSize();
    }
}

void GeodeticGridStructure::determineChunksPerDimension() {
    //Round the integer division up so we have a partially filled chunk at the end
    timeDimChunks = (times.size() / parameters.timeChunkSize) + (times.size() % parameters.timeChunkSize != 0);
    depthDimChunks = (depths.size()[0] / parameters.depthChunkSize) + (depths.size()[0] % parameters.depthChunkSize != 0);
    latDimChunks = (latitudes.size() / parameters.latChunkSize) + (latitudes.size() % parameters.latChunkSize != 0);
    lonDimChunks = (longitudes.size() / parameters.lonChunkSize) + (longitudes.size() % parameters.lonChunkSize != 0);
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

bool GeodeticGridStructure::indexInRange(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex) {
    if(timeIndex >= times.size() ||
       depthIndex >= depths.size()[0] ||
       latIndex >= latitudes.size() ||
       lonIndex >= longitudes.size()) {
        return false;
    }
    return true;
}

bool GeodeticGridStructure::timeInModel(double time) {
    return times[0] <= time && time <= times[times.size() - 1];
}

bool GeodeticGridStructure::depthInModel(Point point) {
    double depth = interpolateWaterColumnDepth(point);

    return 0 >= point.z && point.z >= -depth;
}

bool GeodeticGridStructure::xyInModel(Point point) {
    return longitudes[0] <= point.x && point.x <= longitudes[longitudes.size() - 1] && 
           latitudes[0] <= point.y && point.y <= latitudes[latitudes.size() - 1];
}

double GeodeticGridStructure::indexWaterColumnDepth(unsigned int latIndex, unsigned int lonIndex) {
    return waterColumnDepth.index({latIndex, lonIndex});
}

double GeodeticGridStructure::interpolateWaterColumnDepth(Point point) {
    std::map<std::pair<unsigned int, unsigned int>, double> xyWeights = getXYInterpolationWeights(point);

    double depth = 0;
    for (auto const& xyWeight : xyWeights) {

        depth += waterColumnDepth.index({xyWeight.first.first, xyWeight.first.second}) * xyWeight.second;
    }

    return depth;
}

std::vector<GeodeticGridStructure::ModelFile>& GeodeticGridStructure::getModelFiles() {
    return modelFiles;
}

std::map<unsigned int, double> GeodeticGridStructure::getTimeInterpolationWeights(double time) {
    // Search for first element x such that i ≤ x
    auto firstElementGreater = std::lower_bound(times.begin(), times.end(), time);

     //We should have previously checked if the time is in the range, so just assert if we are outside that range.
    assert(firstElementGreater != times.end());

    int firstElementGreaterIndex = std::distance(times.begin(), firstElementGreater);
    
    //If the time is exactly on the first time, then we need to increment the count so we don't underflow.
    if(firstElementGreaterIndex == 0 && times[firstElementGreaterIndex] == time) {
        firstElementGreaterIndex += 1;
    }

    int beforeIndex = firstElementGreaterIndex - 1;
    int afterIndex = firstElementGreaterIndex;
    double beforeTime = times[beforeIndex];
    double afterTime = times[afterIndex];

    double beforeWeight = (afterTime - time) / (afterTime - beforeTime);
    double afterWeight = 1 - beforeWeight;

    std::map<unsigned int, double> weights;
    weights.insert({beforeIndex, beforeWeight});
    weights.insert({afterIndex, afterWeight});
    return weights;
}


double GeodeticGridStructure::xyMDegLon(double latOrigin) {
    double latOriginRad = latOrigin * M_PI / 180.0;
    return (111415.13 * std::cos(latOriginRad)
            - 94.55 * std::cos(3.0 * latOriginRad)
            - 0.12 * std::cos(5.0 * latOriginRad));
}

double GeodeticGridStructure::xyMDegLat(double latOrigin) {
    double latOriginRad = latOrigin * M_PI / 180.0;
    return (111132.09 - 566.05 * std::cos(2.0 * latOriginRad)
            + 1.20 * std::cos(4.0 * latOriginRad)
            - 0.002 * std::cos(6.0 * latOriginRad));
}


Point GeodeticGridStructure::latLonToLocalXY(Point origin, Point point) {
    double mDegLon = xyMDegLon(origin.y);
    double mDegLat = xyMDegLat(origin.y);

    double x = (point.x - origin.x) * mDegLon;
    double y = (point.y - origin.y) * mDegLat;

    return Point(x, y, point.z);
}

std::map<std::pair<unsigned int, unsigned int>, double> GeodeticGridStructure::getXYInterpolationWeights(Point point) {
    // Search for first element x such that i ≤ x
    auto latFirstElementGreater = std::lower_bound(latitudes.begin(), latitudes.end(), point.y);
    auto lonFirstElementGreater = std::lower_bound(longitudes.begin(), longitudes.end(), point.x);

    //We should have previously checked if the lat and lon is in the range, so just assert if we are outside that range.
    assert(latFirstElementGreater != latitudes.end());
    assert(lonFirstElementGreater != longitudes.end());


    int latFirstElementGreaterIndex = std::distance(latitudes.begin(), latFirstElementGreater);
    int lonFirstElementGreaterIndex = std::distance(longitudes.begin(), lonFirstElementGreater);

    //If the lat is exactly on the first lat, then we need to increment the count so we don't underflow.
    if(latFirstElementGreaterIndex == 0 && latitudes[latFirstElementGreaterIndex] == point.y) {
        latFirstElementGreaterIndex += 1;
    }

    //If the lat is exactly on the first lat, then we need to increment the count so we don't underflow.
    if(lonFirstElementGreaterIndex == 0 && longitudes[lonFirstElementGreaterIndex] == point.x) {
        lonFirstElementGreaterIndex += 1;
    }


    int latBeforeIndex = latFirstElementGreaterIndex - 1;
    int latAfterIndex = latFirstElementGreaterIndex;
    double latBefore = latitudes[latBeforeIndex];
    double latAfter = latitudes[latAfterIndex];

    int lonBeforeIndex = lonFirstElementGreaterIndex - 1;
    int lonAfterIndex = lonFirstElementGreaterIndex;
    double lonBefore = longitudes[lonBeforeIndex];
    double lonAfter = longitudes[lonAfterIndex];

    std::vector<Point> tri1XY;
    std::vector<std::pair<unsigned int, unsigned int>> tri1Index;

    std::vector<Point> tri2XY;
    std::vector<std::pair<unsigned int, unsigned int>> tri2Index;

    Point origin = Point(latBefore, lonBefore,0);
    Point xyPoint = latLonToLocalXY(origin, point);

    tri1Index.push_back(std::make_pair(latBeforeIndex, lonBeforeIndex));
    tri1XY.push_back(latLonToLocalXY(origin, Point(lonBefore, latBefore, 0)));

    tri1Index.push_back(std::make_pair(latBeforeIndex, lonAfterIndex));
    tri1XY.push_back(latLonToLocalXY(origin, Point(lonAfter, latBefore, 0)));

    tri1Index.push_back(std::make_pair(latAfterIndex, lonBeforeIndex));
    tri1XY.push_back(latLonToLocalXY(origin, Point(lonBefore, latAfter, 0)));

    auto tri1Barycenter = calculateBarycentricCoordinates(tri1XY[0], tri1XY[1], tri1XY[2], xyPoint);

    tri2Index.push_back(std::make_pair(latAfterIndex, lonAfterIndex));
    tri2XY.push_back(latLonToLocalXY(origin, Point(lonAfter, latAfter, 0)));

    tri2Index.push_back(std::make_pair(latBeforeIndex, lonAfterIndex));
    tri2XY.push_back(latLonToLocalXY(origin, Point(lonAfter, latBefore, 0)));

    tri2Index.push_back(std::make_pair(latAfterIndex, lonBeforeIndex));
    tri2XY.push_back(latLonToLocalXY(origin, Point(lonBefore, latAfter, 0)));

    auto tri2Barycenter = calculateBarycentricCoordinates(tri2XY[0], tri2XY[1], tri2XY[2], xyPoint);

    std::map<std::pair<unsigned int, unsigned int>, double> weights;
    if(std::get<0>(tri1Barycenter) >= 0 && std::get<1>(tri1Barycenter) >= 0 && std::get<2>(tri1Barycenter) >= 0) {
        weights.insert({tri1Index[0], std::get<0>(tri1Barycenter)});
        weights.insert({tri1Index[1], std::get<1>(tri1Barycenter)});
        weights.insert({tri1Index[2], std::get<2>(tri1Barycenter)});

    } else {
        assert(std::get<0>(tri2Barycenter) >= 0 && std::get<1>(tri2Barycenter) >= 0 && std::get<2>(tri2Barycenter) >= 0);
        weights.insert({tri2Index[0], std::get<0>(tri2Barycenter)});
        weights.insert({tri2Index[1], std::get<1>(tri2Barycenter)});
        weights.insert({tri2Index[2], std::get<2>(tri2Barycenter)});
    }

    return weights;
}

double GeodeticGridStructure::interpolateDepthLayer(std::map<std::pair<unsigned int, unsigned int>, double> xyWeights, unsigned int layer) {
    double val = 0;

    for (auto const& xyWeight : xyWeights) {
        val += depths.index({layer, xyWeight.first.first, xyWeight.first.second}) * xyWeight.second;
    }

    return val;
}

std::map<unsigned int, double> GeodeticGridStructure::getDepthInterpolationWeights(Point point, std::map<std::pair<unsigned int, unsigned int>, double> xyWeights) {

    std::vector<double> interpDepths;

    for(unsigned int i = 0; i < depths.size()[0]; i++) {
        interpDepths.push_back(interpolateDepthLayer(xyWeights, i));
    }

    int depthIndexShallow = 0; //The larger number
    int depthIndexDeep = 0; //The smaller number

    //If depth[0] is the surface
    if(interpDepths[0] > interpDepths[1]) {
        if(point.z > interpDepths[0]) {
            depthIndexShallow = 0;
            depthIndexDeep = 0;
        } else if(point.z < interpDepths[interpDepths.size() - 1]) {
            depthIndexShallow = interpDepths.size() - 1;
            depthIndexDeep = interpDepths.size() - 1;
        } else {
            for(int i = 0; i < interpDepths.size() - 1; i++) {
                if(interpDepths[i] >= point.z && point.z >= interpDepths[i+1]) {
                    depthIndexShallow = i;
                    depthIndexDeep = i+1;
                }
            }
        }
    } else {
        //If depth[0] is the seafloor
        if(point.z < interpDepths[0]) {
            depthIndexShallow = 0;
            depthIndexDeep = 0;
        } else if(point.z > interpDepths[interpDepths.size() - 1]) {
            depthIndexShallow = interpDepths.size() - 1;
            depthIndexDeep = interpDepths.size() - 1;
        } else {
            for(int i = 0; i < interpDepths.size() - 1; i++) {

                if(interpDepths[i] <= point.z && point.z <= interpDepths[i+1]) {
                    depthIndexShallow = i+1;
                    depthIndexDeep = i;
                }
            }
        }
    }

    std::map<unsigned int, double> weights;
    if(depthIndexShallow ==depthIndexDeep) {
        weights.insert({depthIndexShallow, 1.0});
    } else {
        double shallowDepth = interpDepths[depthIndexShallow];
        double deepDepth = interpDepths[depthIndexDeep];

        double shallowWeight = (deepDepth - point.z) / (deepDepth - shallowDepth);
        double deepWeight = 1 - shallowWeight;

        weights.insert({depthIndexShallow, shallowWeight});
        weights.insert({depthIndexDeep, deepWeight});
    }

    return weights;
}

std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>, double> GeodeticGridStructure::getDataInterpolationWeights(Point point, double time) {
    std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>, double> weights;


    std::cout << timeInModel(time) << " " << depthInModel(point) << " " << xyInModel(point) << std::endl;
    if(!timeInModel(time) || !depthInModel(point) || !xyInModel(point)) {
        throw std::out_of_range("GeodeticGrid request outside of model extent");
    }

    std::map<unsigned int, double> timeWeights = getTimeInterpolationWeights(time);
    std::map<std::pair<unsigned int, unsigned int>, double> xyWeights = getXYInterpolationWeights(point);
    std::map<unsigned int, double> depthWeights = getDepthInterpolationWeights(point, xyWeights);

    for (auto const& timeWeight : timeWeights) {
        for (auto const& depthWeight : depthWeights) {
            for (auto const& xyWeights : xyWeights) {
                unsigned int timeIndex = timeWeight.first;
                unsigned int depthIndex = depthWeight.first;
                unsigned int latIndex = xyWeights.first.first;
                unsigned int lonIndex = xyWeights.first.second;
                auto index = std::make_tuple(timeIndex, depthIndex, latIndex, lonIndex);

                double weight = timeWeight.second * depthWeight.second * xyWeights.second;
                weights.insert({index, weight});
            }
        }
    }

    return weights;
}
