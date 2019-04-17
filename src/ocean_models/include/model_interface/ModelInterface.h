#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include "model_interface/ModelData.h"

class ModelInterface
{
public:

	ModelInterface() {}
	virtual ~ModelInterface() {}

	virtual const ModelData getData(double x, double y, double height, double time)=0;
    virtual const ModelData getDataOutOfRange(double x, double y, double height, double time)=0;
};

#endif