#include "ocean_model_interfaces_py/ModelDataPyInterface.h"
#include "ocean_model_interfaces_py/GeodeticGridParametersPyInterface.h"

#include "ocean_model_interfaces/model_interface/ModelData.h"
#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/geodetic_grid/GeodeticGrid.h"
#include "ocean_model_interfaces/geodetic_grid/GeodeticGridParameters.h"

#include <stdio.h>
#include <math.h>

extern "C"
{

void* newGeodeticGrid(struct GeodeticGridParametersPy parameters) {
    ocean_model_interfaces::GeodeticGridParameters cxxParameters;
    cxxParameters.modelDirectory = std::string(parameters.modelDirectory);
    cxxParameters.timeChunkSize = parameters.timeChunkSize;
    cxxParameters.depthChunkSize = parameters.depthChunkSize;
    cxxParameters.latChunkSize = parameters.latChunkSize;
    cxxParameters.lonChunkSize = parameters.lonChunkSize;
    cxxParameters.cacheSize = parameters.cacheSize;
    //return 0;
    return new(std::nothrow) ocean_model_interfaces::GeodeticGrid(cxxParameters);
}

void deleteGeodeticGrid(void* ptr) {
    try
    {
     //   ocean_model_interfaces::GeodeticGrid * ref = reinterpret_cast<ocean_model_interfaces::GeodeticGrid *>(ptr);
      //  delete ref;
    } catch(...) {
        //Don't throw any exceptions because ctypes does not handle them well.
    }
}

ModelDataPy getDataGeodeticGrid(void* ptr, double x, double y, double z, double time) {
    struct ModelDataPy returnModelData;
    returnModelData.u = NAN;
    returnModelData.v = NAN;
    returnModelData.w = NAN;
    returnModelData.temp = NAN;
    returnModelData.salt = NAN;
    returnModelData.dye = NAN;
    returnModelData.depth = NAN;

    ocean_model_interfaces::ModelInterface * ref = reinterpret_cast<ocean_model_interfaces::ModelInterface *>(ptr);

    try{
        ocean_model_interfaces::ModelData data = ref->getData(x,y,z,time);
        returnModelData.u = data.u;
        returnModelData.v = data.v;
        returnModelData.w = data.w;
        returnModelData.temp = data.temp;
        returnModelData.salt = data.salt;
        returnModelData.dye = data.dye;
        returnModelData.depth = data.depth;
    } catch(std::out_of_range) {

    }


    return returnModelData;
}

ModelDataPy getDataOutOfRangeGeodeticGrid(void* ptr, double x, double y, double z, double time) {
    struct ModelDataPy returnModelData;
    returnModelData.u = NAN;
    returnModelData.v = NAN;
    returnModelData.w = NAN;
    returnModelData.temp = NAN;
    returnModelData.salt = NAN;
    returnModelData.dye = NAN;
    returnModelData.depth = NAN;

    ocean_model_interfaces::ModelInterface * ref = reinterpret_cast<ocean_model_interfaces::ModelInterface *>(ptr);

    ocean_model_interfaces::ModelData data = ref->getDataOutOfRange(x,y,z,time);

    returnModelData.u = data.u;
    returnModelData.v = data.v;
    returnModelData.w = data.w;
    returnModelData.temp = data.temp;
    returnModelData.salt = data.salt;
    returnModelData.dye = data.dye;
    returnModelData.depth = data.depth;

    return returnModelData;
}


}
