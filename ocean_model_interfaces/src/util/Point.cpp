#include "ocean_model_interfaces/util/Point.h"
#include <math.h>

using namespace ocean_model_interfaces;

Point::Point() {}

Point::Point(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
