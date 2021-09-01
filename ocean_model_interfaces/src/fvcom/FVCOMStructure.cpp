#include "ocean_model_interfaces/fvcom/FVCOMStructure.h"
#include "ocean_model_interfaces/fvcom/FVCOM.h"

#include <netcdf>
#include <memory>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iterator>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

using namespace ocean_model_interfaces;

FVCOMStructure::FVCOMStructure() {}

FVCOMStructure::FVCOMStructure(const std::string filename, int xChunkSize, int yChunkSize, int siglayChunkSize, int timeChunkSize) :
    xChunkSize(xChunkSize),
    yChunkSize(yChunkSize),
    siglayChunkSize(siglayChunkSize),
    timeChunkSize(timeChunkSize),
    lastContainingTriangle(0)
{
    loadStructureData(filename);

    splitIntoChunks();
}

FVCOMStructure::Plane::Plane() {}

FVCOMStructure::Plane::Plane(FVCOMStructure::Point& p0, FVCOMStructure::Point& p1, FVCOMStructure::Point& p2)
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

double FVCOMStructure::Plane::getHeight(Point& interpolatePoint)
{
    return (-d - a * interpolatePoint.x - b * interpolatePoint.y) / c;
}

std::vector<std::string> FVCOMStructure::traverseDataFiles(const std::string filename)
{
    std::vector<std::string> filenames;
    fs::path p1 = filename;
    if(fs::is_directory(p1))
    {
        for(auto& p: fs::directory_iterator(p1))
        {
            if(!fs::is_directory(p))
            {
                filenames.push_back(p.path().string());
            }
        }
    }
    else
    {
        filenames.push_back(filename);
    }
    return filenames;
}

void FVCOMStructure::loadStructureData(const std::string directory)
{
    unsigned int timeDim = 0;
    std::vector<std::string> filenames = traverseDataFiles(directory);

    //Set start times and time dimensions from files
    for(auto &filename : filenames)
    {
        netCDF::NcFile dataFile(filename, netCDF::NcFile::read);
        timeDim += dataFile.getDim("time").getSize();
        
        std::vector<float> tempTimes;
        tempTimes.resize(dataFile.getDim("time").getSize());
        netCDF::NcVar timeVar = dataFile.getVar("time");
        timeVar.getVar(tempTimes.data());
        ModelFile modelFile;
        modelFile.filename = filename;
        modelFile.startTime = tempTimes[0];
        modelFile.timeDim = dataFile.getDim("time").getSize();
        modelFiles.push_back(modelFile);
    }
    
    //sort filenames based on start ties
    std::sort(modelFiles.begin(), modelFiles.end());

    //load time variables into one vector
    times.resize(timeDim);
    unsigned int currentIndex = 0;
    for(auto &modelFile : modelFiles)
    {
        netCDF::NcFile dataFile(modelFile.filename, netCDF::NcFile::read);
        netCDF::NcVar timeVar = dataFile.getVar("time");

        //Set the start time index for this file
        modelFile.startTimeIndex = currentIndex;

        //Load times from this file
        timeVar.getVar(times.data() + currentIndex);

        //move current index for next files
        currentIndex += dataFile.getDim("time").getSize();
    }
    netCDF::NcFile dataFile(modelFiles[0].filename, netCDF::NcFile::read);

    //Get dimensions of structure elements
    unsigned int nodeDim = dataFile.getDim("node").getSize();
    unsigned int neleDim = dataFile.getDim("nele").getSize();
    siglayDim = dataFile.getDim("siglay").getSize();

    //Load all variables for the structure of the model
    netCDF::NcVar xVar = dataFile.getVar("x");
    netCDF::NcVar yVar = dataFile.getVar("y");
    netCDF::NcVar xcVar = dataFile.getVar("xc");
    netCDF::NcVar ycVar = dataFile.getVar("yc");
    netCDF::NcVar nvVar = dataFile.getVar("nv");
    netCDF::NcVar hVar = dataFile.getVar("h");
    netCDF::NcVar siglayVar = dataFile.getVar("siglay");
    netCDF::NcVar centerSiglayVar = dataFile.getVar("siglay_center");

    //Load in as floats as that is what they are in the files
    //but they will be converted to doubles to maintain precision during calculations
    std::vector<float> nodeX;
    std::vector<float> nodeY;
    std::vector<float> nodeH;

    std::vector<float> triangleX;
    std::vector<float> triangleY;

    nodeX.resize(nodeDim);
    nodeY.resize(nodeDim);
    triangleX.resize(neleDim);
    triangleY.resize(neleDim);
    nodeH.resize(nodeDim);

    //Resize siglay 2d vectors
    nodeSiglay.resize(nodeDim);

    for(unsigned int i = 0; i < nodeSiglay.size(); i++)
    {
        nodeSiglay[i].resize(siglayDim);
    }

    //resize for multidimensional array
    triangleToNodes.resize(neleDim);
    
    for(unsigned int i = 0; i < neleDim; i++)
    {
        triangleToNodes[i].resize(3);
    }

    //Assign all arrays for the structure variables
    xVar.getVar(nodeX.data());
    yVar.getVar(nodeY.data());
    xcVar.getVar(triangleX.data());
    ycVar.getVar(triangleY.data());
    hVar.getVar(nodeH.data());

    //load nvVar into a multidimensional vector
    for(unsigned int i = 0; i < neleDim; i++)
    {
        std::vector<size_t> start = {0, i};
        std::vector<size_t> count = {3, 1};
        nvVar.getVar(start, count, triangleToNodes[i].data());
    }

    //load node siglay into a multidimensional vector
    for(unsigned int i = 0; i < nodeSiglay.size(); i++)
    {
        std::vector<size_t> start = {0, i};
        std::vector<size_t> count = {siglayDim, 1};
        siglayVar.getVar(start, count, nodeSiglay[i].data());
    }

    //Convert to use point struct
    nodes.resize(nodeDim);
    triangles.resize(neleDim);
    for(unsigned int i = 0; i < nodeDim; i++)
    {
        nodes[i].x = nodeX[i];
        nodes[i].y = nodeY[i];
        nodes[i].z = nodeH[i];
    }

    for(unsigned int i = 0; i < neleDim; i++)
    {
        triangles[i].x = triangleX[i];
        triangles[i].y = triangleY[i];
        triangles[i].z = std::numeric_limits<double>::quiet_NaN();
        //Height data is not loaded for triangles so we set it to NaN.
    }


    //The nv variable from the netCDF indexes starting at 1
    //Convert this to 0 by subtracting 1 from every value
    for(unsigned int i = 0; i < neleDim; i++)
    {
        for(unsigned int j = 0; j < 3; j++)
        {
            triangleToNodes[i][j]--;
        }
    }

    //Pre Processes model to get node to triangle conversion
    nodeToTriangles.resize(nodeDim);

    for(unsigned int i = 0; i < neleDim; i++)
    {
        for(unsigned int j = 0; j < 3; j++)
        {
            int triangle = i;
            int node = triangleToNodes[i][j];

            nodeToTriangles[node].push_back(triangle);
        }
    }

}

void FVCOMStructure::splitIntoChunks()
{
    getModelExtent();

    siglayDimChunks = std::ceil(siglayDim / (double)siglayChunkSize);
    timeDimChunks = std::ceil(times.size() / (double)timeChunkSize);
    yDimChunks = std::ceil((maxY - minY) / (double)yChunkSize);
    xDimChunks = std::ceil((maxX - minX) / (double)xChunkSize);

    nodesInChunk.resize(xDimChunks * yDimChunks);
    trianglesInChunk.resize(xDimChunks * yDimChunks);
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        FVCOMStructure::ChunkInfo chunk = getChunkForNode(i, 0, 0);

        //This does not contain the siglay or time dimensions as the
        //node locations do not change with depth or time.
        int chunkId = chunk.yChunk + (chunk.xChunk * yDimChunks);

        nodesInChunk[chunkId].push_back(i);
    }

    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        FVCOMStructure::ChunkInfo chunk = getChunkForTriangle(i, 0, 0);

        //This does not contain the siglay or time dimensions as the
        //node locations do not change with depth or time.
        int chunkId = chunk.yChunk + (chunk.xChunk * yDimChunks);

        trianglesInChunk[chunkId].push_back(i);
    }
}

void FVCOMStructure::getModelExtent()
{
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        Point& node = nodes[i];
        
        if(node.x > maxX)
        {
            maxX = node.x;
        }

        if(node.x < minX)
        {
            minX = node.x;
        }

        if(node.y > maxY)
        {
            maxY = node.y;
        }

        if(node.y < minY)
        {
            minY = node.y;
        }
    }
}

bool FVCOMStructure::pointInTriangle(Point testPoint, int triangle) const
{
    int p0Index = triangleToNodes[triangle][0];
    int p1Index = triangleToNodes[triangle][1];
    int p2Index = triangleToNodes[triangle][2];

    Point p0 = nodes[p0Index];
    Point p1 = nodes[p1Index];
    Point p2 = nodes[p2Index];

    //Calculate barycentric coordinates
    double alpha = ((p1.y - p2.y)*(testPoint.x - p2.x) + (p2.x - p1.x)*(testPoint.y - p2.y)) /
        ((p1.y - p2.y)*(p0.x - p2.x) + (p2.x - p1.x)*(p0.y - p2.y));

    double beta = ((p2.y - p0.y)*(testPoint.x - p2.x) + (p0.x - p2.x)*(testPoint.y - p2.y)) /
           ((p1.y - p2.y)*(p0.x - p2.x) + (p2.x - p1.x)*(p0.y - p2.y));

    double gamma = 1.0 - alpha - beta;

    //if all coordinates are none negative then the point is in the triangle
    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

int FVCOMStructure::getContainingTriangle(Point testPoint, int closestNode)
{
    if(pointInTriangle(testPoint, lastContainingTriangle))
    {
        return lastContainingTriangle;
    }

    //Search all triangles that are connected to the closest node
    for(unsigned int i = 0; i < nodeToTriangles[closestNode].size(); i++)
    {
        //return the triangle for which the point is inside
        if(pointInTriangle(testPoint, nodeToTriangles[closestNode][i]))
        {
            lastContainingTriangle = nodeToTriangles[closestNode][i];
            return nodeToTriangles[closestNode][i];
        }
    }

    //if the point is not inside any of those triangles search all the triangles
    for(unsigned int i = 0; i < triangles.size(); i++)
    {
        if(pointInTriangle(testPoint, i))
        {
            lastContainingTriangle = i;
            return i;
        }
    }


    throw std::out_of_range("FVCOM request outside of model extent");
}

int FVCOMStructure::getContainingTriangle(Point testPoint)
{
    if(pointInTriangle(testPoint, lastContainingTriangle))
    {
        return lastContainingTriangle;
    }

    //Get the closest node to start the search for the containing triangle
    int closestNode = getClosestNode(testPoint);

    return getContainingTriangle(testPoint, closestNode);
}

const std::vector<int>& FVCOMStructure::getNodesInTriangle(int triangle) const
{
    return triangleToNodes[triangle];
}

const std::vector<FVCOMStructure::ModelFile> FVCOMStructure::getModelFiles() const
{
    return modelFiles;
}

int FVCOMStructure::getClosestNode(Point testPoint) const
{
    //Checks distance between testPoint and every node, this is slow and will probably need to be improved
    double closestDistance = std::numeric_limits<double>::max();
    int node = -1;
    for(unsigned int i = 0; i < nodes.size(); i++)
    {

        if(distanceSquared(testPoint, nodes[i]) < closestDistance)
        {
            closestDistance = distanceSquared(testPoint, nodes[i]);
            node = i;
        }
    }

    return node;
}


int FVCOMStructure::getClosestTime(double time) const
{
    auto lower = std::lower_bound(times.begin(), times.end(), time);

    int index1 = std::distance(times.begin(), lower);
    int index2 = index1 - 1;


    if(std::abs(times[index1] - time) > std::abs(times[index2] - time))
    {
        return index2;
    }
    else
    {
        return index1;
    }

}

int FVCOMStructure::getPreviousTimeIndex(double time) const
{
    auto lower = std::lower_bound(times.begin(), times.end(), time);
    int index = std::distance(times.begin(), lower);

    if(times[index] == time)
     {
         return index;
    }

    return index - 1;
}

float FVCOMStructure::getTime(int timeIndex) const
{
    return times[timeIndex];
}

FVCOMStructure::Plane FVCOMStructure::getTrianglePlane(int triangle) const
{
    const std::vector<int>& surroundingNodes = triangleToNodes[triangle];

    FVCOMStructure::Point p0 = getNodePointWithH(surroundingNodes[0]);
    FVCOMStructure::Point p1 = getNodePointWithH(surroundingNodes[1]);
    FVCOMStructure::Point p2 = getNodePointWithH(surroundingNodes[2]);
    
    FVCOMStructure::Plane plane(p0, p1, p2);

    return plane;
}

FVCOMStructure::Plane FVCOMStructure::getTriangleSiglayPlane(int triangle, unsigned int siglay) const
{
    const std::vector<int>& surroundingNodes = triangleToNodes[triangle];

    FVCOMStructure::Point p0 = getNodePointAtSiglay(surroundingNodes[0], siglay);
    FVCOMStructure::Point p1 = getNodePointAtSiglay(surroundingNodes[1], siglay);
    FVCOMStructure::Point p2 = getNodePointAtSiglay(surroundingNodes[2], siglay);
    
    FVCOMStructure::Plane plane(p0, p1, p2);

    return plane;
}

double FVCOMStructure::distance(Point p0, Point p1) const
{
    return std::sqrt((p0.x - p1.x)*(p0.x - p1.x) + (p0.y - p1.y)*(p0.y - p1.y) );
}

double FVCOMStructure::distanceSquared(Point p0, Point p1) const
{
    return (p0.x - p1.x)*(p0.x - p1.x) + (p0.y - p1.y)*(p0.y - p1.y);
}

const FVCOMStructure::Point FVCOMStructure::getNodePointWithH(int node) const
{
    return nodes[node];
}

const FVCOMStructure::Point FVCOMStructure::getNodePointAtSiglay(int node, int siglay) const
{
    FVCOMStructure::Point returnPoint = nodes[node];
    returnPoint.z = returnPoint.z * nodeSiglay[node][siglay];

    return returnPoint;
}


FVCOMStructure::ChunkInfo FVCOMStructure::getChunkForNode(int node, int siglay, int time) const
{
    //Chunk ids based on this ordering (x,y,sigma,time)
    FVCOMStructure::ChunkInfo chunk;

    double nodeX = nodes[node].x;
    double nodeY = nodes[node].y;

    //calculate the chunks for each individual dimension
    chunk.xChunk = (nodeX - minX) / xChunkSize;
    chunk.yChunk = (nodeY - minY) / yChunkSize;
    chunk.siglayChunk = siglay / siglayChunkSize;
    chunk.timeChunk = time / timeChunkSize;

    //Check to insure that the chunks are valid.  If not this node is in the last chunk dimension.
    //NOTE: This should only occur if the x/y extent is divisible by x/y chunk dimension.
    //In this case maxX and maxY will give a chunk# as 1 more than the last chunk index.
    //It seems like a waste to have the a chunk only be these single nodes so they are included
    //in the last chunk.
    if(chunk.xChunk >= xDimChunks)
    {
        chunk.xChunk = xDimChunks - 1;
    }

    if(chunk.yChunk >= yDimChunks)
    {
        chunk.yChunk = yDimChunks - 1;
    }

    chunk.id = chunk.timeChunk +
           (chunk.siglayChunk * timeDimChunks) +
           (chunk.yChunk * timeDimChunks * siglayDimChunks) +
           (chunk.xChunk * timeDimChunks * siglayDimChunks * yDimChunks);


    chunk.xStart = chunk.xChunk * xChunkSize - minX;
    chunk.yStart = chunk.yChunk * yChunkSize - minX;
    chunk.siglayStart = chunk.siglayChunk * siglayChunkSize;
    chunk.timeStart = chunk.timeChunk * timeChunkSize;

    chunk.xSize = xChunkSize;
    chunk.ySize = yChunkSize;

    unsigned int timeSize = times.size();
    chunk.siglaySize = std::min(siglayChunkSize, siglayDim - chunk.siglayStart);
    chunk.timeSize = std::min(timeChunkSize, timeSize - chunk.timeStart);

    return chunk;
}


FVCOMStructure::ChunkInfo FVCOMStructure::getChunkForTriangle(int triangle, int siglay, int time) const
{
    //Chunk ids based on this ordering (x,y,sigma,time)

    FVCOMStructure::ChunkInfo chunk;

    double triangleX = triangles[triangle].x;
    double triangleY = triangles[triangle].y;

    //calculate the chunks for each individual dimension


    chunk.xChunk = (triangleX - minX) / xChunkSize;
    chunk.yChunk = (triangleY - minY) / yChunkSize;
    chunk.siglayChunk = siglay / siglayChunkSize;
    chunk.timeChunk = time / timeChunkSize;

    //Check to insure that the chunks are valid.  If not this node is in the last chunk dimension.
    //NOTE: This should only occur if the x/y extent is divisible by x/y chunk dimension.
    //In this case maxX and maxY will give a chunk# as 1 more than the last chunk index.
    //It seems like a waste to have the a chunk only be these single nodes so they are included
    //in the last chunk.

    if(chunk.xChunk >= xDimChunks)
    {
        chunk.xChunk = xDimChunks - 1;
    }

    if(chunk.yChunk >= yDimChunks)
    {
        chunk.yChunk = yDimChunks - 1;
    }

    chunk.id = chunk.timeChunk +
           (chunk.siglayChunk * timeDimChunks) +
           (chunk.yChunk * timeDimChunks * siglayDimChunks) +
           (chunk.xChunk * timeDimChunks * siglayDimChunks * yDimChunks);

    chunk.xStart = chunk.xChunk * xChunkSize - minX;
    chunk.yStart = chunk.yChunk * yChunkSize - minX;
    chunk.siglayStart = chunk.siglayChunk * siglayChunkSize;
    chunk.timeStart = chunk.timeChunk * timeChunkSize;

    chunk.xSize = xChunkSize;
    chunk.ySize = yChunkSize;

    unsigned int timeSize = times.size();
    chunk.siglaySize = std::min(siglayChunkSize, siglayDim - chunk.siglayStart);
    chunk.timeSize = std::min(timeChunkSize, timeSize - chunk.timeStart);

    return chunk;
}

void FVCOMStructure::timeInterpolation(double time, int& time1Index, int& time2Index, double& time1Percent) const
{
    time1Index = getPreviousTimeIndex(time);
    double previousTime = getTime(time1Index);

    //Time is exactly on a time division, no interpolation needed.
    if(previousTime == time)
    {
        time2Index = time1Index;
        time1Percent = 1;
    }
    else //time is spilt between divisions so it needs interpolation
    {
        time2Index = time1Index + 1;
        double nextTime = getTime(time2Index);
        time1Percent = (nextTime - time) / (nextTime - previousTime);
    }
}


void FVCOMStructure::siglayInterpolation(FVCOMStructure::Point& interpolatePoint, int& siglay1Index, int& siglay2Index, double& siglay1Percent)
{
    int containingTriangle = getContainingTriangle(interpolatePoint);
    siglayInterpolation(interpolatePoint, siglay1Index, siglay2Index, siglay1Percent, containingTriangle);
}

void FVCOMStructure::siglayInterpolation(FVCOMStructure::Point& interpolatePoint, int& siglay1Index, int& siglay2Index, double& siglay1Percent, int containingTriangle)
{
    siglay1Index = siglay2Index = -1;

    double prevDot = 0;
    for(uint i = 0; i < getNumSiglays(); i++)
    {
        FVCOMStructure::Plane plane = getTriangleSiglayPlane(containingTriangle, i);

        //calculate the dot product with the plane and the point to determine which side of the plane it is on
        double dot = plane.a * interpolatePoint.x + plane.b * interpolatePoint.y + plane.c * interpolatePoint.z + plane.d;

        if(i != 0)
        {
            if(dot == 0) //point is in the plane, use current siglayIndex and no interpolation needed
            {
                siglay1Index = i;
                siglay2Index = i;
                break;
            }
            else if((dot > 0 && prevDot < 0) || //The sign of dot has changed so the siglay has been found
                    (dot < 0 && prevDot > 0))
            {
                siglay1Index = i - 1;
                siglay2Index = i;
                break;
            }
            else if((prevDot > 0 && prevDot < dot) || //The distance from the plane to the point is increasing so we have passed it.
                    (prevDot < 0 && prevDot > dot))   //If this occurs then it means the point is above the 0th siglay, use the 0th siglay
            {
                siglay1Index = 0;
                siglay2Index = 0;
                break;
            }
        }

        prevDot = dot;
    }


    //The dot product was always decreasing however it never changed sign.
    //Therefore the point is below the final siglay, so use the final siglay.
    if(siglay1Index == siglay2Index && siglay2Index == -1)
    {
        siglay1Index = getNumSiglays() -1;
        siglay2Index = getNumSiglays() -1;
    }

    if(siglay1Index == siglay2Index) //The point is above the 0th siglay so and there is no data there
    {
        siglay1Percent = 1.0;
    }
    else
    {
        FVCOMStructure::Plane upperPlane = getTriangleSiglayPlane(containingTriangle, siglay1Index);
        FVCOMStructure::Plane lowerPlane = getTriangleSiglayPlane(containingTriangle, siglay2Index);

        double upperH = (-upperPlane.d - upperPlane.a * interpolatePoint.x - upperPlane.b * interpolatePoint.y) / upperPlane.c;
        double lowerH = (-lowerPlane.d - lowerPlane.a * interpolatePoint.x - lowerPlane.b * interpolatePoint.y) / lowerPlane.c;

        siglay1Percent = (lowerH - interpolatePoint.z) / (lowerH - upperH);
    }
}

double FVCOMStructure::getDepthAtPoint(FVCOMStructure::Point& interpolatePoint, int containingTriangle)
{
    const std::vector<int>& surroundingNodes = getNodesInTriangle(containingTriangle);

    FVCOMStructure::Point p1 = getNodePointWithH(surroundingNodes[0]);
    FVCOMStructure::Point p2 = getNodePointWithH(surroundingNodes[1]);
    FVCOMStructure::Point p3 = getNodePointWithH(surroundingNodes[2]);

    FVCOMStructure::Plane groundPlane(p1,p2,p3);

    return groundPlane.getHeight(interpolatePoint);
}

double FVCOMStructure::getDepthAtPoint(FVCOMStructure::Point& interpolatePoint)
{
    int containingTriangle = getContainingTriangle(interpolatePoint);

    const std::vector<int>& surroundingNodes = getNodesInTriangle(containingTriangle);

    FVCOMStructure::Point p1 = getNodePointWithH(surroundingNodes[0]);
    FVCOMStructure::Point p2 = getNodePointWithH(surroundingNodes[1]);
    FVCOMStructure::Point p3 = getNodePointWithH(surroundingNodes[2]);

    FVCOMStructure::Plane groundPlane(p1,p2,p3);

    return groundPlane.getHeight(interpolatePoint);
}

const bool FVCOMStructure::pointInModel(Point p, double time)
{
    int containingTriangle = 0;
    try
    {
        containingTriangle = getContainingTriangle(p);
    }
    catch(const std::out_of_range& e)
    {
        return false;
    }

    Plane plane = getTrianglePlane(containingTriangle);
    double depth = plane.getHeight(p);
    return p.x >= minX && p.x <= maxX && 
           p.y >= minY && p.y <= maxY && 
           time >= times[0] && time <= times[times.size() - 1] &&
           p.z <= 0 && p.z >= -depth;
}

const bool FVCOMStructure::timeInModel(double time) const
{
    return time >= times[0] && time <= times[times.size() - 1];
}

const bool FVCOMStructure::depthInModel(Point p)
{
    int containingTriangle = 0;
    try
    {
        containingTriangle = getContainingTriangle(p);
    }
    catch(const std::out_of_range& e)
    {
        return false;
    }

    Plane plane = getTrianglePlane(containingTriangle);
    double depth = plane.getHeight(p);

    return p.z <= 0 && p.z >= -depth;
}

const bool FVCOMStructure::xyInModel(Point p) const
{
    return p.x >= minX && p.x <= maxX && 
           p.y >= minY && p.y <= maxY;
}


const std::vector<unsigned int>& FVCOMStructure::getNodesInChunk(FVCOMStructure::ChunkInfo chunk) const
{
    int chunkId = chunk.yChunk + (chunk.xChunk * yDimChunks);

    return nodesInChunk[chunkId];
}

const std::vector<unsigned int>& FVCOMStructure::getTrianglesInChunk(FVCOMStructure::ChunkInfo chunk) const
{
    int chunkId = chunk.yChunk + (chunk.xChunk * yDimChunks);

    return trianglesInChunk[chunkId];
}

const unsigned int FVCOMStructure::getNumSiglays() const
{
    return siglayDim;
}
