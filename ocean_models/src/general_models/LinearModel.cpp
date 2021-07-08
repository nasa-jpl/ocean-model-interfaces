#include "ocean_models/general_models/LinearModel.h"
#include "ocean_models/model_interface/ModelData.h"

#include <string>
#include <stdexcept>
#include <math.h>

using namespace ocean_models;

LinearModel::LinearModel() {}

LinearModel::LinearModel(Parameters parameters) :
    parameters(parameters)
{}

const ModelData LinearModel::getDataHelper(double x, double y, double height, double time)
{
    ModelData data;
    
    double distance = 0;

    if(parameters.type == LinearModel::DistanceFunction::EUCLIDEAN)
    {
        distance = sqrt((x - parameters.centerX) * (x - parameters.centerX) + (y - parameters.centerY) * (y - parameters.centerY) + (height - parameters.centerZ) * (height - parameters.centerZ));
    }
    else if(parameters.type == LinearModel::DistanceFunction::MANHATTAN)
    {
        distance = std::max(fabs(x - parameters.centerX), fabs(y - parameters.centerY));
    }
    data.u = std::max(0.0,parameters.u * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.v = std::max(0.0,parameters.v * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.temp = std::max(0.0,parameters.temp * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.salt = std::max(0.0,parameters.salt * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.dye = std::max(0.0,parameters.dye * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.depth = parameters.depth; 

    return data;
}

const ModelData LinearModel::getDataOutOfRangeHelper(double x, double y, double height, double time)
{
    ModelData data;
    
    double distance = 0;

    if(parameters.type == LinearModel::DistanceFunction::EUCLIDEAN)
    {
        distance = sqrt((x - parameters.centerX) * (x - parameters.centerX) + (y - parameters.centerY) * (y - parameters.centerY) + (height - parameters.centerZ) * (height - parameters.centerZ));
    }
    else if(parameters.type == LinearModel::DistanceFunction::MANHATTAN)
    {
        distance = std::max(fabs(x - parameters.centerX), fabs(y - parameters.centerY));
    }

    data.u = std::max(0.0,parameters.u * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.v = std::max(0.0,parameters.v * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.temp = std::max(0.0,parameters.temp * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.salt = std::max(0.0,parameters.salt * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.dye = std::max(0.0,parameters.dye * (parameters.zeroDistance - distance) / parameters.zeroDistance);
    data.depth = parameters.depth; 

    return data;
}