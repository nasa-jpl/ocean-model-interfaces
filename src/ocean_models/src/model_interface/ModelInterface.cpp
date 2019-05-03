#include "ocean_models/model_interface/ModelInterface.h"

#include <stdexcept>
#include <math.h>
#include  <limits>

using namespace ocean_models;

#define SECONDS_IN_DAY 86400

ModelInterface::ModelInterface() :
	offsetX(0),
	offsetY(0),
	offsetHeight(0),
	offsetTime(0)
{}

ModelInterface::~ModelInterface() {}

const ModelData ModelInterface::getData(double x, double y, double height, double time)
{
	return this->getDataHelper(x - offsetX, y - offsetY, height - offsetHeight, time - offsetTime);
}

const ModelData ModelInterface::getDataOutOfRange(double x, double y, double height, double time)
{
	return this->getDataOutOfRangeHelper(x - offsetX, y - offsetY, height - offsetHeight, time - offsetTime);
}

void ModelInterface::setOffsets(double offsetX, double offsetY, double offsetHeight, double offsetTime)
{
	this->offsetX = offsetX;
	this->offsetY = offsetY;
	this->offsetHeight = offsetHeight;
	this->offsetTime = offsetTime;
}
