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

namespace ocean_model_interfaces
{

/**
 * Class used to load and query the structure of GeodeticGrid data. This data is always stored in memory
 * and is used to determine what chunks need to be loaded to retrieve specific parts of the model.
 */
class GeodeticGridStructure
{
public:

    GeodeticGridStructure(GeodeticGridParameters parameters);

    struct Point
    {
        double x;
        double y;
        double z;
    };

    /**
     * Stores a 3d plane. Used to determine the direction of a point relative to a
     * specific siglay.
     */
    struct Plane
    {
        double a;
        double b;
        double c;
        double d;

        Plane();
        Plane(GeodeticGridStructure::Point& p0, GeodeticGridStructure::Point& p1, GeodeticGridStructure::Point& p2);

        double getHeight(Point& interpolatePoint);
    };

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

    bool isInRange(unsigned int timeIndex, unsigned int depthIndex, unsigned int latIndex, unsigned int lonIndex);

    double getWaterColumnDepth(unsigned int latIndex, unsigned int lonIndex);

    std::vector<ModelFile>& getModelFiles();

private:
    void loadStructureData();
    void loadTime();
    void determineChunksPerDimension();

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