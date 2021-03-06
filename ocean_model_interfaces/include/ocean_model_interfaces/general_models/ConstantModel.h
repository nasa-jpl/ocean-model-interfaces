#ifndef CONSTANT_MODEL_H
#define CONSTANT_MODEL_H

#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

namespace ocean_model_interfaces
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

    const ModelData getDataHelper(double x, double y, double z, double time);
    const ModelData getDataOutOfRangeHelper(double x, double y, double z, double time);

    struct Parameters
    {
        Parameters() {};

        double u = 0;
        double v = 0;
        double temp = 0;
        double salt = 0;
        double dye = 0;
        double depth = 100;
    };
private:
    Parameters parameters;

};

}
#endif
