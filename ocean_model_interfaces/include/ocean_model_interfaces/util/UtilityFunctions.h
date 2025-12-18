#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <vector>
#include <string>

#include "ocean_model_interfaces/util/Point.h"

namespace ocean_model_interfaces
{

std::vector<std::string> traverseDataFiles(const std::string filename);

std::tuple<double, double, double> calculateBarycentricCoordinates(Point p0, Point p1, Point p2, Point testPoint);

Point latLonToLocalXY(Point origin, Point point);
Point localXYToLatLon(Point origin, Point point);
double xyMDegLon(double latOrigin);
double xyMDegLat(double latOrigin);

}

#endif