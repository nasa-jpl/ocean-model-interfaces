#include "ocean_model_interfaces/util/Plane.h"
#include <math.h>

using namespace ocean_model_interfaces;

Plane::Plane() {}

Plane::Plane(Point& p0, Point& p1, Point& p2)
{
    double ab[3];
    double ac[3];

    ab[0] = p1.x - p0.x;
    ab[1] = p1.y - p0.y;
    ab[2] = p1.z - p0.z;

    ac[0] = p2.x - p0.x;
    ac[1] = p2.y - p0.y;
    ac[2] = p2.z - p0.z;

    a = (ab[1] * ac[2]) - (ab[2] * ac[1]);
    b = (ab[2] * ac[0]) - (ab[0] * ac[2]);
    c = (ab[0] * ac[1]) - (ab[1] * ac[0]);

    double magnitude = sqrt(a * a +  b * b +  c * c);
    a /= magnitude;
    b /= magnitude;
    c /= magnitude;

    d = -(p0.x * a + p0.y * b + p0.z * c);
}

double Plane::getHeight(Point& interpolatePoint)
{
    return (-d - a * interpolatePoint.x - b * interpolatePoint.y) / c;
}

