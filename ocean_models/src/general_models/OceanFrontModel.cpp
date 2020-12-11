#include "ocean_models/general_models/OceanFrontModel.h"
#include "ocean_models/model_interface/ModelData.h"

#include <stdexcept>
#include <math.h>

using namespace ocean_models;

OceanFrontModel::OceanFrontModel() {}
OceanFrontModel::OceanFrontModel(Parameters parameters) :
    parameters(parameters)
{}

const ModelData OceanFrontModel::getDataHelper(double x, double y, double height, double time)
{
    double depth = std::abs(height); //Unsure if height is positive or negative...

    ModelData data;

    double linePoint2X = parameters.frontX + std::cos(parameters.frontOrientation) * 10;
    double linePoint2Y = parameters.frontY + std::sin(parameters.frontOrientation) * 10;

    double distanceToLine = std::abs((linePoint2X - parameters.frontX) * (parameters.frontY - y) - (parameters.frontX - x) * (linePoint2Y - parameters.frontY)) / 
                            std::sqrt(std::pow(linePoint2X - parameters.frontX, 2) + std::pow(linePoint2Y - parameters.frontY, 2));

    double side = sign((linePoint2X - parameters.frontX) * (y - parameters.frontY) - (linePoint2Y - parameters.frontY) * (x - parameters.frontX));

    //Get interp percents for the sides
    double interpPercentSide1 = 1;
    if(distanceToLine >= parameters.frontWidth / 2) {
        if(side < 0) {
            interpPercentSide1 = 1;
        } else {
            interpPercentSide1 = 0;
        }
    } else {
        if(side < 0) {
            interpPercentSide1 = (distanceToLine / (parameters.frontWidth / 2)) * 0.5 + 0.5;
        } else {
            interpPercentSide1 = 1 - ((distanceToLine / (parameters.frontWidth / 2)) * 0.5 + 0.5);
        }
    }
    double interpPercentSide2 = 1 - interpPercentSide1;
    
    //Get indicies for the depths
    double depthIndex1 = 0;
    for(uint i = 0; i < parameters.depths.size(); i++) {
        if(parameters.depths[i] < depth) {
            depthIndex1 = i;
        }
    }
    double depthIndex2 = depthIndex1 + 1;

    //Get interp percents for the depths
    double interpPercentDepth1 = 0;
    if(depthIndex2 >= parameters.depths.size()) {
        interpPercentDepth1 = 1.0;
        depthIndex2 = depthIndex1;
    } else {
        interpPercentDepth1 = 1 - ((depth - parameters.depths[depthIndex1]) / (parameters.depths[depthIndex2] - parameters.depths[depthIndex1]));
    }
    double interpPercentDepth2 = 1 - interpPercentDepth1;

    double side1Temp = parameters.side1Temps[depthIndex1] * interpPercentDepth1 + parameters.side1Temps[depthIndex2] * interpPercentDepth2;
    double side2Temp = parameters.side2Temps[depthIndex1] * interpPercentDepth1 + parameters.side2Temps[depthIndex2] * interpPercentDepth2;
    double side1Salt = parameters.side1Salts[depthIndex1] * interpPercentDepth1 + parameters.side1Salts[depthIndex2] * interpPercentDepth2;
    double side2Salt = parameters.side2Salts[depthIndex1] * interpPercentDepth1 + parameters.side2Salts[depthIndex2] * interpPercentDepth2;

    data.temp = side1Temp * interpPercentSide1 + side2Temp * interpPercentSide2;
    data.salt = side1Salt * interpPercentSide1 + side2Salt * interpPercentSide2;

    data.u = parameters.currentU;
    data.v = parameters.currentV;
    data.dye = parameters.dye;
    data.depth = parameters.depths[parameters.depths.size() - 1];

    return data;
}

const ModelData OceanFrontModel::getDataOutOfRangeHelper(double x, double y, double height, double time)
{
    return getDataHelper(x, y, height, time);
}

double OceanFrontModel::sign(double x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

OceanFrontModel::Parameters::Parameters() {}

OceanFrontModel::Parameters::Parameters(underwater_autonomy::ConfigurationFile& config) {
    frontX = config.readSimpleEntry<double>("front_x", frontX);
    frontY = config.readSimpleEntry<double>("front_y", frontY);
    frontOrientation = config.readSimpleEntry<double>("front_orientation", frontOrientation);
    frontWidth = config.readSimpleEntry<double>("front_width", frontWidth);

    depths = config.readArrayEntry<double>("depths", depths);
    side1Temps = config.readArrayEntry<double>("side1_temp", side1Temps);
    side2Temps = config.readArrayEntry<double>("side2_temp", side2Temps);
    side1Salts = config.readArrayEntry<double>("side1_salt", side1Salts);
    side2Salts = config.readArrayEntry<double>("side2_salt", side2Salts);

    currentU = config.readSimpleEntry<double>("current_u", currentU);
    currentV = config.readSimpleEntry<double>("current_v", currentV);
    dye = config.readSimpleEntry<double>("dye", dye);
}