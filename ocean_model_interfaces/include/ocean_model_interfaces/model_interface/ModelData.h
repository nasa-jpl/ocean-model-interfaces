#ifndef MODEL_DATA_H
#define MODEL_DATA_H

namespace ocean_model_interfaces
{

/**
 * Data from a model at one specific location (x,y,z) and time.
 */
struct ModelData
{
    /** Eastward Current Velocity */
    double u;

    /** Northward Current Velocity */
    double v;

    /**Upward Current Velocity */
    double w;
    
    double temp;
    double salt;
    double dye;

    /** Bathymetry depth at the specific (x,y) location. This should be positive for locations that have water. */
    double depth;
};

}
#endif