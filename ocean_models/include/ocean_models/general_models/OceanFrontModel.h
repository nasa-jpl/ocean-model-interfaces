#ifndef OCEAN_FRONT_MODEL_H
#define OCEAN_FRONT_MODEL_H

#include "ocean_models/model_interface/ModelInterface.h"
#include "ocean_models/model_interface/ModelData.h"

#include "underwater_autonomy/util/ConfigurationFile.h"

namespace ocean_models
{

/**
 * Class used to load and query FVCOM data
 */
class OceanFrontModel : public ModelInterface
{
public:
    struct Parameters; //Forward declare Parameters so we can use it in the constructor

    /**
     * Initalize ConstantModel class with zeros
     */
    OceanFrontModel();

    /**
     * Initalize ConstantModel class with provided values
     */
    OceanFrontModel(Parameters parameters);

    const ModelData getDataHelper(double x, double y, double height, double time);
    const ModelData getDataOutOfRangeHelper(double x, double y, double height, double time);

    struct Parameters
    {
        Parameters();
        Parameters(underwater_autonomy::ConfigurationFile& config);

        double frontX = 0;
        double frontY = 0;
        double frontOrientation = 0;
        double frontWidth = 1000;

        std::vector<double> depths = {0,5,10,15,20,25,30};
        std::vector<double> side1Temps = {16.5,16.4,16.3,16.2,16.1,16,15.9};
        std::vector<double> side2Temps = {16.5,16.4,16.3,16.2,16.1,16,15.9};
        std::vector<double> side1Salts = {30,30,30,30,30,30,30};
        std::vector<double> side2Salts = {30,30,30,30,30,30,30};

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
