#ifndef PLANE_H
#define PLANE_H

#include "ocean_model_interfaces/util/Point.h"

namespace ocean_model_interfaces
{

/**
 * Stores a 3d plane. Used to determine the direction of a point relative to a
 * specific siglay.
 */
struct Plane
{
    double a;
    double b;
    double c;
    double d;

    Plane();
    Plane(Point& p0, Point& p1, Point& p2);

    double getHeight(Point& interpolatePoint);
};

}
#endif