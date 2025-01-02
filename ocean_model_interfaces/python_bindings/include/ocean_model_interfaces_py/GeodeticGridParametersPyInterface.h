#ifndef GEODETIC_GRID_PARAMETERS_PY_INTERFACE
#define GEODETIC_GRID_PARAMETERS_PY_INTERFACE

extern "C"
{
    struct GeodeticGridParametersPy {
        const char* modelDirectory;
        const unsigned int timeChunkSize = 1;
        const unsigned int depthChunkSize = 40;
        const unsigned int latChunkSize = 10;
        const unsigned int lonChunkSize = 10;
        const unsigned int cacheSize = 10;
    };
}

#endif