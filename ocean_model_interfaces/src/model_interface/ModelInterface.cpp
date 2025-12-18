#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/util/UtilityFunctions.h"

#include <stdexcept>
#include <math.h>
#include  <limits>
#include <assert.h>
#include <iostream>

using namespace ocean_model_interfaces;

#define SECONDS_IN_DAY 86400

ModelInterface::ModelInterface() :
    offsetX(0),
    offsetY(0),
    offsetZ(0),
    offsetTime(0)
{}

ModelInterface::~ModelInterface() {}

const ModelData ModelInterface::getData(double x, double y, double z, double time)
{

    if(positionType == CoordinateType::XY) {

        return this->getDataHelper(x + offsetX, y + offsetY, z + offsetZ, time + offsetTime);

    } else {
        assert(positionType == CoordinateType::LATLON);

        //Convert the lat lon to xy based on the origin and shift based on the offset        
        Point pointXY = latLonToLocalXY(origin, Point(x,y,z));

        return this->getDataHelper(pointXY.x + offsetX, pointXY.y + offsetY, z + offsetZ, time + offsetTime);
    }
}

const ModelData ModelInterface::getDataOutOfRange(double x, double y, double z, double time)
{
    if(positionType == CoordinateType::XY) {
        return this->getDataOutOfRangeHelper(x + offsetX, y + offsetY, z + offsetZ, time + offsetTime);

    } else {
        assert(positionType == CoordinateType::LATLON);

        //Convert the lat lon to xy based on the origin and shift based on the offset        
        Point pointXY = latLonToLocalXY(origin, Point(x,y,z));


        return this->getDataOutOfRangeHelper(pointXY.x + offsetX, pointXY.y + offsetY, z + offsetZ, time + offsetTime);
    }
}

void ModelInterface::setOffsets(double offsetX, double offsetY, double offsetZ, double offsetTime)
{
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->offsetZ = offsetZ;
    this->offsetTime = offsetTime;
}

void ModelInterface::setOrigin(Point origin) {
    this->origin = origin;
}

void ModelInterface::setCoordinateType(CoordinateType type) {
    this->positionType = type;
}
