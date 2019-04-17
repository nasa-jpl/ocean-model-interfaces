#include "general_models/ConstantModel.h"
#include "model_interface/ModelData.h"

#include <stdexcept>
#include <math.h>

ConstantModel::ConstantModel() :
 	u(0),
 	v(0),
 	temp(0),
 	salt(0),
 	dye(0)
{}

ConstantModel::ConstantModel(double u, double v, double temp, double salt, double dye, double depth) :
 	u(u),
 	v(v),
 	temp(temp),
 	salt(salt),
 	dye(dye),
    depth(depth)
{}

const ModelData ConstantModel::getData(double x, double y, double height, double time)
{
	ModelData data;
	
	data.u = u;
	data.v = v;
	data.temp = temp;
	data.salt = salt;
	data.dye = dye;
    data.depth = depth;

	return data;
}

const ModelData ConstantModel::getDataOutOfRange(double x, double y, double height, double time)
{
	ModelData data;
	
	data.u = u;
	data.v = v;
	data.temp = temp;
	data.salt = salt;
	data.dye = dye;
    data.depth = depth;

	return data;
}