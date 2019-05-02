#include "general_models/LinearModel.h"
#include "model_interface/ModelData.h"

#include <string>
#include <stdexcept>
#include <math.h>

using namespace ocean_models;

LinearModel::LinearModel() :
    u(0),
    v(0),
    temp(0),
    salt(0),
    dye(0),
    zeroDistance(100),
    centerX(0),
    centerY(0),
    centerZ(0),
    type("circle")
{}

LinearModel::LinearModel(float u, float v, float temp, float salt, float dye, float depth,
                            float zeroDistance, float centerX, float centerY, float centerZ, std::string type) :
    u(u),
    v(v),
    temp(temp),
    salt(salt),
    dye(dye),
    depth(depth),
    zeroDistance(zeroDistance),
    centerX(centerX),
    centerY(centerY),
    centerZ(centerZ),
    type(type)
{}

const ModelData LinearModel::getDataHelper(double x, double y, double height, double time)
{
    ModelData data;
    
    double distance = 0;

    if(type == "circle")
    {
        distance = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) + (height - centerZ) * (height - centerZ));
    }
    else if(type == "square")
    {
        distance = std::max(fabs(x - centerX), fabs(y - centerY));
    }
    data.u = std::max(0.0,u * (zeroDistance - distance) / zeroDistance);
    data.v = std::max(0.0,v * (zeroDistance - distance) / zeroDistance);
    data.temp = std::max(0.0,temp * (zeroDistance - distance) / zeroDistance);
    data.salt = std::max(0.0,salt * (zeroDistance - distance) / zeroDistance);
    data.dye = std::max(0.0,dye * (zeroDistance - distance) / zeroDistance);
    data.depth = depth; 

    return data;
}

const ModelData LinearModel::getDataOutOfRangeHelper(double x, double y, double height, double time)
{
    ModelData data;
    
    double distance = 0;

    if(type == "circle")
    {
        distance = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) + (height - centerZ) * (height - centerZ));
    }
    else if(type == "square")
    {
        distance = std::max(fabs(x - centerX), fabs(y - centerY));
    }

    data.u = std::max(0.0,u * (zeroDistance - distance) / zeroDistance);
    data.v = std::max(0.0,v * (zeroDistance - distance) / zeroDistance);
    data.temp = std::max(0.0,temp * (zeroDistance - distance) / zeroDistance);
    data.salt = std::max(0.0,salt * (zeroDistance - distance) / zeroDistance);
    data.dye = std::max(0.0,dye * (zeroDistance - distance) / zeroDistance);
    data.depth = depth; 

    return data;
}