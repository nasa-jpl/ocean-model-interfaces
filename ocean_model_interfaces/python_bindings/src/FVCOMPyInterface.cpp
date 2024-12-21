#include "ocean_model_interfaces_py/ModelDataPyInterface.h"

#include "ocean_model_interfaces/model_interface/ModelData.h"
#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/fvcom/FVCOM.h"

#include <stdio.h>    

extern "C"
{

void* newFVCOM(const char* filename) {

    return new(std::nothrow) ocean_model_interfaces::FVCOM(std::string(filename));
}

void deleteFVCOM(void* ptr) {
    try
    {
        ocean_model_interfaces::FVCOM * ref = reinterpret_cast<ocean_model_interfaces::FVCOM *>(ptr);
        delete ref;
    } catch(...) {
        //Don't throw any exceptions because ctypes does not handle them well.
    }
}

ModelDataPy getDataFVCOM(void* ptr, double x, double y, double z, double time) {
    struct ModelDataPy returnModelData;
    returnModelData.u = -1;
    returnModelData.v = -1;
    returnModelData.w = -1;
    returnModelData.temp = -1;
    returnModelData.salt = -1;
    returnModelData.dye = -1;
    returnModelData.depth = -1;

    ocean_model_interfaces::ModelInterface * ref = reinterpret_cast<ocean_model_interfaces::ModelInterface *>(ptr);

    ocean_model_interfaces::ModelData data = ref->getData(x,y,z,time);

    returnModelData.u = data.u;
    returnModelData.v = data.v;
    returnModelData.w = data.w;
    returnModelData.temp = data.temp;
    returnModelData.salt = data.salt;
    returnModelData.dye = data.dye;
    returnModelData.depth = data.depth;

    return returnModelData;
}


ModelDataPy getDataOutOfRangeFVCOM(void* ptr, double x, double y, double z, double time) {
    struct ModelDataPy returnModelData;
    returnModelData.u = -1;
    returnModelData.v = -1;
    returnModelData.w = -1;
    returnModelData.temp = -1;
    returnModelData.salt = -1;
    returnModelData.dye = -1;
    returnModelData.depth = -1;

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