#ifndef LINEAR_MODEL_H
#define LINEAR_MODEL_H

#include <string>

#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

namespace ocean_model_interfaces
{

/**
 * A model used for testing that provides a linear drop off from the given values to 0.
 * Centered on an origin.
 */
class LinearModel : public ModelInterface
{
public:
    struct Parameters; //Forward declare Parameters so we can use it in the constructor

    /**
     * Determines the distance function to use when calculating the dropoff.
     */ 
    enum DistanceFunction {EUCLIDEAN, MANHATTAN};

    /**
     * Initalize LinearModel class with zeros
     */
    LinearModel();

    /**
     * Initalize LinearModel class with provided parameters
     */
    LinearModel(Parameters parameters);

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
        double zeroDistance = 100;
        double centerX = 0;
        double centerY = 0;
        double centerZ = 0;
        DistanceFunction type = DistanceFunction::EUCLIDEAN;
    };

private:
    Parameters parameters;
};

}
#endif
