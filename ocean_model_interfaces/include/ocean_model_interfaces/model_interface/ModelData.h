#ifndef MODEL_DATA_H
#define MODEL_DATA_H

namespace ocean_model_interfaces
{

/**
 * Data from a model at one specific location (x,y,z) and time.
 */
struct ModelData
{
    //eastward current velocity
    double u;

    //northward current velocity
    double v;

    //upward current velocity
    double w;
    
    double temp;
    double salt;
    double dye;
    double depth;
};

}
#endif