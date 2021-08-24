#include "ocean_model_interfaces/general_models/ConstantModel.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

#include <stdexcept>
#include <math.h>

using namespace ocean_models;

ConstantModel::ConstantModel() {}

ConstantModel::ConstantModel(Parameters parameters) :
    parameters(parameters)
{}

const ModelData ConstantModel::getDataHelper(double x, double y, double z, double time)
{
    ModelData data;
    
    data.u = parameters.u;
    data.v = parameters.v;
    data.temp = parameters.temp;
    data.salt = parameters.salt;
    data.dye = parameters.dye;
    data.depth = parameters.depth;

    return data;
}

const ModelData ConstantModel::getDataOutOfRangeHelper(double x, double y, double z, double time)
{
    ModelData data;
    
    data.u = parameters.u;
    data.v = parameters.v;
    data.temp = parameters.temp;
    data.salt = parameters.salt;
    data.dye = parameters.dye;
    data.depth = parameters.depth;

    return data;
}