#ifndef GEODETIC_GRID_PARAMETERS_H
#define GEODETIC_GRID_PARAMETERS_H

#include <string>

struct GeodeticGridParameters {
    //The directory that the model is contained in.
    std::string modelDirectory;

    //The size of the chunks to load in number of indicies
    unsigned int timeChunkSize = 1;
    unsigned int depthChunkSize = 40;
    unsigned int latChunkSize = 10;
    unsigned int lonChunkSize = 10;
};

#endif