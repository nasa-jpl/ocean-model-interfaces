#ifndef LINEAR_MODEL_H
#define LINEAR_MODEL_H

#include <string>

#include "model_interface/ModelInterface.h"
#include "model_interface/ModelData.h"

namespace ocean_models
{

/**
 * Class used to load and query FVCOM data
 */
class LinearModel : public ModelInterface
{
public:

    /**
     * Initalize ConstantModel class with zeros
     */
    LinearModel();

    /**
     * Initalize ConstantModel class with provided values
     */
    LinearModel(float u, float v, float temp, float salt, float dye, float depth,
                  float zeroDistance, float centerX, float centerY, float centerZ, std::string type);



    const ModelData getData(double x, double y, double height, double time);
    const ModelData getDataOutOfRange(double x, double y, double height, double time);

private:
    
    double u;
    double v;
    double temp;
    double salt;
    double dye;
    double depth;
    double zeroDistance;
    double centerX;
    double centerY;
    double centerZ;
    std::string type;

};

}
#endif
