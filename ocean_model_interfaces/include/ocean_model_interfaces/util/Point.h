#ifndef POINT_H
#define POINT_H

namespace ocean_model_interfaces
{

struct Point
{
    Point();
    Point(double x, double y, double z);

    double x;
    double y;
    double z;
};

}
#endif