#include "ocean_model_interfaces/util/UtilityFunctions.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <math.h>

namespace fs = boost::filesystem;

std::vector<std::string> ocean_model_interfaces::traverseDataFiles(const std::string filename)
{
    std::vector<std::string> filenames;
    fs::path p1 = filename;
    if(fs::is_directory(p1))
    {
        for(auto& p: fs::directory_iterator(p1))
        {
            if(!fs::is_directory(p))
            {
                if(p.path().string().find(".nc") != std::string::npos) {
                    filenames.push_back(p.path().string());
                }
            }
        }
    }
    else
    {
        filenames.push_back(filename);
    }
    return filenames;
}

std::tuple<double, double, double> ocean_model_interfaces::calculateBarycentricCoordinates(ocean_model_interfaces::Point p0, 
                                                                                           ocean_model_interfaces::Point p1, 
                                                                                           ocean_model_interfaces::Point p2, 
                                                                                           ocean_model_interfaces::Point testPoint) {
    //Calculate barycentric coordinates
    double alpha = ((p1.y - p2.y)*(testPoint.x - p2.x) + (p2.x - p1.x)*(testPoint.y - p2.y)) /
        ((p1.y - p2.y)*(p0.x - p2.x) + (p2.x - p1.x)*(p0.y - p2.y));

    double beta = ((p2.y - p0.y)*(testPoint.x - p2.x) + (p0.x - p2.x)*(testPoint.y - p2.y)) /
           ((p1.y - p2.y)*(p0.x - p2.x) + (p2.x - p1.x)*(p0.y - p2.y));

    double gamma = 1.0 - alpha - beta;

    return std::make_tuple(alpha, beta, gamma);
}


double ocean_model_interfaces::xyMDegLon(double latOrigin) {
    double latOriginRad = latOrigin * M_PI / 180.0;
    return (111415.13 * std::cos(latOriginRad)
            - 94.55 * std::cos(3.0 * latOriginRad)
            - 0.12 * std::cos(5.0 * latOriginRad));
}

double ocean_model_interfaces::xyMDegLat(double latOrigin) {
    double latOriginRad = latOrigin * M_PI / 180.0;
    return (111132.09 - 566.05 * std::cos(2.0 * latOriginRad)
            + 1.20 * std::cos(4.0 * latOriginRad)
            - 0.002 * std::cos(6.0 * latOriginRad));
}


ocean_model_interfaces::Point ocean_model_interfaces::latLonToLocalXY(ocean_model_interfaces::Point origin, ocean_model_interfaces::Point point) {
    double mDegLon = xyMDegLon(origin.y);
    double mDegLat = xyMDegLat(origin.y);

    double x = (point.x - origin.x) * mDegLon;
    double y = (point.y - origin.y) * mDegLat;

    return Point(x, y, point.z);
}

ocean_model_interfaces::Point ocean_model_interfaces::localXYToLatLon(ocean_model_interfaces::Point origin, ocean_model_interfaces::Point point) {
    double mDegLon = xyMDegLon(origin.y);
    double mDegLat = xyMDegLat(origin.y);

    double x = (point.x / mDegLon) + origin.x;
    double y = (point.y / mDegLat) + origin.y;

    return ocean_model_interfaces::Point(x, y, point.z);
}