#ifndef CONSTANT_MODEL_H
#define CONSTANT_MODEL_H

#include "ocean_models/model_interface/ModelInterface.h"
#include "ocean_models/model_interface/ModelData.h"

namespace ocean_models
{

/**
 * A model used for testing that provides constant data independent of time and location.
 */
class ConstantModel : public ModelInterface
{
public:
    struct Parameters; //Forward declare Parameters so we can use it in the constructor

    /**
     * Initalize ConstantModel class with all zero values
     */
    ConstantModel();

    /**
     * Initalize ConstantModel class with provided values
     */
    ConstantModel(Parameters parameters);

    const ModelData getDataHelper(double x, double y, double height, double time);
    const ModelData getDataOutOfRangeHelper(double x, double y, double height, double time);

    struct Parameters
    {
        Parameters() {};

        double u = 0;
        double v = 0;
        double temp = 0;
        double salt = 0;
        double dye = 0;
        double depth = 10;
    };
private:
    Parameters parameters;

};

}
#endif
