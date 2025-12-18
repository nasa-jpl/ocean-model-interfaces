#ifndef GEODETIC_GRID_PARAMETERS_H
#define GEODETIC_GRID_PARAMETERS_H

#include <string>
#include <functional>

namespace ocean_model_interfaces
{

struct GeodeticGridParameters {
    //The directory that the model is contained in.
    std::string modelDirectory;

    //The size of the chunks to load in number of indicies
    unsigned int timeChunkSize = 1;
    unsigned int depthChunkSize = 10;
    unsigned int latChunkSize = 10;
    unsigned int lonChunkSize = 10;

    //The size of the cache used for storing loaded chunks
    unsigned int cacheSize = 10;

    //Functions called when starting or ending loading model from disk.
    std::function<void(void)> startLoad;
    std::function<void(void)> endLoad;
};
}
#endif