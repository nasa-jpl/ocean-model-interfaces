#include "ocean_model_interfaces_py/ModelDataPyInterface.h"

#include "ocean_model_interfaces/model_interface/ModelData.h"
#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/fvcom/FVCOM.h"

#include "ocean_model_interfaces/util/Point.h"

#include <stdio.h>    
#include <math.h>

extern "C"
{

void setOffsets(void* ptr, double offsetX, double offsetY, double offsetZ, double offsetTime) {
    ocean_model_interfaces::ModelInterface * ref = reinterpret_cast<ocean_model_interfaces::ModelInterface *>(ptr);
    ref->setOffsets(offsetX,offsetY,offsetZ,offsetTime);
}

void setOrigin(void* ptr, double longitude, double latitude) {
    ocean_model_interfaces::ModelInterface * ref = reinterpret_cast<ocean_model_interfaces::ModelInterface *>(ptr);
    ref->setOrigin(ocean_model_interfaces::Point(longitude, latitude, 0.0));
}

void setCoordinateType(void* ptr, ocean_model_interfaces::ModelInterface::CoordinateType type) {
    ocean_model_interfaces::ModelInterface * ref = reinterpret_cast<ocean_model_interfaces::ModelInterface *>(ptr);
    ref->setCoordinateType(type);
}

}