#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include "model_interface/ModelData.h"

namespace ocean_models
{

class ModelInterface
{
public:

	ModelInterface();
	virtual ~ModelInterface();

	/**
	* Public interface for getting model data. This will add the specified offsets to x, y, and time
	*
	**/
	const ModelData getData(double x, double y, double height, double time);

	/**
	* Public interface for getting model data when request is out of range. This will add the specified offsets to x, y, and time
	*
	**/
    const ModelData getDataOutOfRange(double x, double y, double height, double time);

	void setOffsets(double offsetX, double offsetY, double offsetHeight, double offsetTime);

protected:

	/**
	* Internal helper to get data. This will be overriden by the different model types
	*/
	virtual const ModelData getDataHelper(double x, double y, double height, double time)=0;

	/**
	* Internal helper to get data when request is out of range. This will be overriden by the different model types
	*/
    virtual const ModelData getDataOutOfRangeHelper(double x, double y, double height, double time)=0;

private:
	double offsetX;
	double offsetY;
	double offsetHeight;
	double offsetTime;
};

}
#endif