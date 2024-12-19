#ifndef GEODETIC_GRID_STRUCTURE_H
#define GEODETIC_GRID_STRUCTURE_H

#include <netcdf>

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <limits>


#include "ocean_model_interfaces/geodetic_grid/GeodeticGridParameters.h"
#include "ocean_model_interfaces/util/MultiDimensionalVector.h"
#include "ocean_model_interfaces/util/Point.h"

namespace ocean_model_interfaces
{

/**
 * Class used to load and query the structure of GeodeticGrid data. This data is always stored in memory
 * and is used to determine what chunks need to be loaded to retrieve specific parts of the model.
 */
class GeodeticGridStructure
{
public:
    GeodeticGridStructure();
    GeodeticGridStructure(GeodeticGridParameters parameters);

    /**
     * Stores information about where a chunk fits in the larger model
     */
    struct ChunkInfo
    {
        //The chunk indicies
        unsigned int id;
        unsigned int lonChunk;
        unsigned int latChunk;
        unsigned int depthChunk;
        unsigned int timeChunk;

        //The model indicies that this chunk starts at
        unsigned int lonStart;
        unsigned int latStart;
        unsigned int depthStart;
        unsigned int timeStart;

        //The size of this chunk in indicies for each dimension
        unsigned int lonSize;
        unsigned int latSize;
        unsigned int depthSize;
        unsigned int timeSize;
    };

    /**
     * Contains information about each netCDF file that makes up the model.
     */
    struct ModelFile
    {
        /** String of the filename for this file */
        std::string filename;

        /** Start time for this file */
        float startTime;

        /** Time index for the start time of this file */
        unsigned int startTimeIndex;

        /** The Time dimension for this file */
        unsigned int timeDim;

        bool operator<(const ModelFile& rhs) const { return startTime < rhs.startTime; }
    };

    /**
     * @brief Get the full ChunkInfo struct for a chunk that the given indicies are in.
     */
    ChunkInfo getGridChunkInfo(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);

    /**
     * @brief Get just the chunkID that the given indicies are in
     */
    unsigned int getChunkIdFromIndicies(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);

    bool indexInRange(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);
    bool timeInModel(double time);
    bool depthInModel(Point point);
    bool xyInModel(Point point);

    double indexWaterColumnDepth(unsigned int latIndex, unsigned int lonIndex);

    double interpolateWaterColumnDepth(Point point);

    std::vector<ModelFile>& getModelFiles();

    std::map<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>, double> getDataInterpolationWeights(Point point, double time);

private:
    void loadStructureData();
    void loadTime();
    void determineChunksPerDimension();

    std::map<unsigned int, double> getTimeInterpolationWeights(double time);
    std::map<unsigned int, double> getDepthInterpolationWeights(Point point, std::map<std::pair<unsigned int, unsigned int>, double> xyWeights);
    std::map<std::pair<unsigned int, unsigned int>, double> getXYInterpolationWeights(Point point);
    Point latLonToLocalXY(Point origin, Point point);
    double interpolateDepthLayer(std::map<std::pair<unsigned int, unsigned int>, double> xyWeights, unsigned int layer);
    double xyMDegLon(double latOrigin);
    double xyMDegLat(double latOrigin);
private:
    std::vector<ModelFile> modelFiles;
    std::vector<double> times;
    std::vector<double> longitudes;
    std::vector<double> latitudes;

    MultiDimensionalVector<double> depths;

    MultiDimensionalVector<double> waterColumnDepth;

    //Size of chunks in different dimensions
    GeodeticGridParameters parameters;

    //Number of chunks for each dimension
    unsigned int depthDimChunks;
    unsigned int timeDimChunks;
    unsigned int latDimChunks;
    unsigned int lonDimChunks;
};

}
#endif