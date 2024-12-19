#include "ocean_model_interfaces/util/UtilityFunctions.h"
#include <boost/filesystem.hpp>
#include <iostream>
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