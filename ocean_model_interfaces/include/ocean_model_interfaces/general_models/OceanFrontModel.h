#ifndef OCEAN_FRONT_MODEL_H
#define OCEAN_FRONT_MODEL_H

#include <vector>

#include "ocean_model_interfaces/model_interface/ModelInterface.h"
#include "ocean_model_interfaces/model_interface/ModelData.h"

namespace ocean_model_interfaces
{

/**
 * Extremely primitive model for ocean fronts. "Front" is assumed to be linear and infinintly long.
 * Model values for either side of the front can be provided.
 */
class OceanFrontModel : public ModelInterface
{
public:
    struct Parameters; //Forward declare Parameters so we can use it in the constructor

    /**
     * Initalize OceanFrontModel class with zeros
     */
    OceanFrontModel();

    /**
     * Initalize OceanFrontModel class with provided values
     */
    OceanFrontModel(Parameters parameters);

    const ModelData getDataHelper(double x, double y, double z, double time);
    const ModelData getDataOutOfRangeHelper(double x, double y, double z, double time);

    struct Parameters
    {
        Parameters() {};

        //Front location, orientation, and width
        double frontX = 0;
        double frontY = 0;
        double frontOrientation = 0;
        double frontWidth = 1000;

        //Temperatures and salts at provided depths on either side of the front.
        //Default setup has no actual front as the values are the same.
        std::vector<double> depths = {0,5,10,15,20,25,30};
        std::vector<double> side1Temps = {16.5,16.4,16.3,16.2,16.1,16,15.9};
        std::vector<double> side2Temps = {16.5,16.4,16.3,16.2,16.1,16,15.9};
        std::vector<double> side1Salts = {30,30,30,30,30,30,30};
        std::vector<double> side2Salts = {30,30,30,30,30,30,30};

        //Other model variables are constant.
        double currentU = 0;
        double currentV = 0;
        double dye = 0;
    };

private:

    double sign(double x);
private:
    Parameters parameters;

};

}
#endif
