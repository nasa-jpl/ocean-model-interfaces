#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include "ocean_models/model_interface/ModelData.h"

namespace ocean_models
{

/**
 * Interface for implementing access to different types of ocean models.
 */
class ModelInterface
{
public:

    ModelInterface();
    virtual ~ModelInterface();

    /**
    * Public interface for retriving model data. This will add the specified offsets to x, y, and time.
    * If requested data is outside of the model then this should throw and out_of_range exception.
    **/
    const ModelData getData(double x, double y, double height, double time);

    /**
    * Public interface for retriving model data when request is out of the model bounds. This will add the specified offsets to x, y, and time.
    *
    **/
    const ModelData getDataOutOfRange(double x, double y, double height, double time);

    /**
     * Set the 4D offset to apply to requested data. This can be used to shift the origin of the model
     * in the world frame.
     */
    void setOffsets(double offsetX, double offsetY, double offsetHeight, double offsetTime);

protected:

    /**
    * Internal helper to retriving data when requested location is in the model bounds. This will be overriden by the different model types.
    */
    virtual const ModelData getDataHelper(double x, double y, double height, double time)=0;

    /**
    * Internal helper to retriving data when request is out of the model bounds. This will be overriden by the different model types
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