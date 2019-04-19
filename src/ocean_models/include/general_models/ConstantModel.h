#ifndef CONSTANT_MODEL_H
#define CONSTANT_MODEL_H

#include "model_interface/ModelInterface.h"
#include "model_interface/ModelData.h"

namespace ocean_models
{

/**
 * Class used to load and query FVCOM data
 */
class ConstantModel : public ModelInterface
{
public:

	/**
	 * Initalize ConstantModel class with zeros
     */
	ConstantModel();

	/**
	 * Initalize ConstantModel class with provided values
     */
	ConstantModel(double u, double v, double temp, double salt, double dye, double depth);



	const ModelData getData(double x, double y, double height, double time);
	const ModelData getDataOutOfRange(double x, double y, double height, double time);

private:
	
	double u;
	double v;
	double temp;
	double salt;
	double dye;
	double depth;

};

}
#endif
