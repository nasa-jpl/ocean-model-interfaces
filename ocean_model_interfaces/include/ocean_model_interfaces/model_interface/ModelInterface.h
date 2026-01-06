#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include "ocean_model_interfaces/model_interface/ModelData.h"
#include "ocean_model_interfaces/util/Point.h"

namespace ocean_model_interfaces
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
    * The input type expected by this function depends on how positionType is set. The base implementation
    * assumes the underlying model accepts a (x,y, height) tuple in meters. If the underlying model is different
    * (i.e. accepts lat/lon) then it should override this function. If requested data is outside of the model then 
    * this should throw and out_of_range exception.
    **/
    virtual const ModelData getData(double x, double y, double z, double time);

    /**
    * Public interface for retriving model data when request is out of the model bounds. This will add the specified offsets to x, y, and time.
    * The input type expected by this function depends on how positionType is set. The base implementation
    * assumes the underlying model accepts a (x,y, height) tuple in meters. If the underlying model is different
    * (i.e. accepts lat/lon) then it should override this function. 
    **/
    virtual const ModelData getDataOutOfRange(double x, double y, double z, double time);

    /**
     * Set the 4D offset to apply to requested data. This can be used to shift the origin of the model
     * in the world frame.
     */
    void setOffsets(double offsetX, double offsetY, double offsetZ, double offsetTime);

    void setOrigin(Point origin);

    enum CoordinateType {
        XY = 0,
        LATLON,
    };
    void setCoordinateType(CoordinateType type);


protected:

    /**
    * Internal helper to retriving data when requested location is in the model bounds. This will be overriden by the different model types.
    * Reference frame is dependent on specific ocean model used, however, for consistency z should always be negative at depth.
    */
    virtual const ModelData getDataHelper(double x, double y, double z, double time)=0;

    /**
    * Internal helper to retriving data when request is out of the model bounds. This will be overriden by the different model types
    * Reference frame is dependent on specific ocean model used, however, for consistency z should always be negative at depth.
    */
    virtual const ModelData getDataOutOfRangeHelper(double x, double y, double z, double time)=0;

    double offsetX;
    double offsetY;
    double offsetZ;
    double offsetTime;

    Point origin;

    /**
     * @brief The type of position that is expected to be passed into the getData and getDataOutOfRange functions
     */
    CoordinateType positionType = CoordinateType::XY;
};

}
#endif