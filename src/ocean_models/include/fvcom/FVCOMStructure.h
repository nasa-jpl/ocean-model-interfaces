#ifndef FVCOM_STRUCTURE_H
#define FVCOM_STRUCTURE_H

#include <list>
#include <unordered_map>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <limits>

#include <netcdf>


/**
 * Class used to load and query FVCOM structure data
 */
class FVCOMStructure
{
public:

	/**
	 * Initalize FVCOMStructure class with data from file and specify the size of the FVCOMChunks
     * @param filename File to load
     * @param xChunkSize Size of a chunk in the x direction
     * @param yChunkSize Size of a chunk in the y direction
     * @param siglayChunkSize Size of a chunk in the siglay direction
     * @param timeChunkSize Size of a chunk in the time direction
     */
	FVCOMStructure(const std::string filename, int xChunkSize, int yChunkSize, int siglayChunkSize, int timeChunkSize);

	/**
	 * Initalize FVCOMStructure class with no data
     */
	FVCOMStructure();

	/**
	 * Struct to group a (x, y, height) point for storing points
     */
	struct Point
	{
		/**
		 * x location in meters
		 */
		double x;

		/**
		 * y location in meters
		 */
		double y;

		/**
		 * water depth (bathymetry) in meters
		 */
		double h;
	};

	struct Plane
	{
		double a;
		double b;
		double c;
		double d;

		Plane();
		Plane(FVCOMStructure::Point& p0, FVCOMStructure::Point& p1, FVCOMStructure::Point& p2);

		double getHeight(Point& interpolatePoint);
	};

	
	

	struct ChunkInfo
	{
		unsigned int id;
		unsigned int xChunk;
		unsigned int yChunk;
		unsigned int siglayChunk;
		unsigned int timeChunk;

		unsigned int xStart;
		unsigned int yStart;
		unsigned int siglayStart;
		unsigned int timeStart;

		unsigned int xSize;
		unsigned int ySize;
		unsigned int siglaySize;
		unsigned int timeSize;
	};

	struct ModelFile
	{
		/**
		 * String of the filename for this file
		 */
		std::string filename;

		/**
		 * Start time for this file
		 */
		float startTime;

		/**
		 * Time index for the start time of this file
		 */
		unsigned int startTimeIndex;

		/**
		 * The Time dimension for this file
		 */
		unsigned int timeDim;

		bool operator<(const ModelFile& rhs) const { return startTime < rhs.startTime; }
	};

	/**
	 * Determines if a point is in the specified triangle
	 * @param testPoint Point to test with
	 * @param triangle Triangle to test with
	 */
	bool pointInTriangle(Point testPoint, int triangle) const;

	/**
	 * Finds the triangle which contains the specified point
	 * @param testPoint Point to get containing triangle for
	 */
	int getContainingTriangle(Point testPoint);
	int getContainingTriangle(Point testPoint, int closestNode);
	/**
	 * Gets the nodes that form the specified triangle
	 * @param triangle Triangle to get the nodes for
	 * @return Vector of the nodes
	 */
	const std::vector<int>& getNodesInTriangle(int triangle) const;

	/**
	 * Gets the equation for the plane defined by a siglay heights of the three nodes of a triangle
	 * @param triangle triangle to get the siglay for
	 * @param siglay to get the plane for
	 * @return parameters defining the plane
	 */
	FVCOMStructure::Plane getTriangleSiglayPlane(int triange, unsigned int siglay) const;

	FVCOMStructure::Plane getTrianglePlane(int triange) const;

	/**
	 * Gets the siglay that is closest to the given location
	 * @param testPoint location to find the closest siglay for
	 * @return index for the closest siglay
	 */
	int getClosestTriangleSiglay(Point testPoint);
	int getClosestTriangleSiglay(Point testPoint, int triangleIndex);
	/**
	 * Finds the closest node to a point
	 * @param testPoint Point to get the closest node for
	 */
	int getClosestNode(Point testPoint) const;
	

	/**
	 * Gets the siglay that is closest to the given location
	 * @param testPoint location to find the closest siglay for
	 * @return index for the closest siglay
	 */
	int getClosestNodeSiglay(Point testPoint) const;

	/**
	 *Gets the point of a node with seafloor depth as height. Note: The z component is depth so it will be positive
	 */
	const FVCOMStructure::Point getNodePoint(int node) const;

	/**
	 *Gets the point of a node at a specific siglay.  Note: The z component is height so it will be negative; this is due to the siglay values being negative.
	 */
	const FVCOMStructure::Point getNodePoint(int node, int siglay) const;

	/**
	 * Gets the time index that is closest to the given time
	 * @param time time to find the closest index for
	 * @return index for the closest time
	 */
	 int getClosestTime(double time) const;

	 /**
	 * Gets the time index that is previous to the given time
	 * @param time time to find the previous index for
	 * @return index for the previous time
	 */
	 int getPreviousTimeIndex(double time) const;

	 /**
	  *Gets time for a specific index
	  */
	 float getTime(int timeIndex) const;

	/**
	 * Gets the distance between two points
	 * @param p0 Point 0 for which to get the distance
	 * @param p1 Point 1 for which  to get the distance
	 */
	double distance(Point p0, Point p1) const;
	double distanceSquared(Point p0, Point p1) const;
	/**
	 * Gets the chunk that contains the (node, sigma, time) tuple
	 * @param node Node to find the chunk for
	 * @param sigma Sigma layer to find the chunk for
	 * @param time Time slice to find the chunk for
	 * @return Chunk which contains the data for the specified node
	 */
	FVCOMStructure::ChunkInfo getChunkForNode(int node, int siglay, int time) const;

	/**
	 * Gets the chunk that contains the (triangle, sigma, time) tuple
	 * @param triangle Triangle to find the chunk for
	 * @param sigma Sigma layer to find the chunk for
	 * @param time Time slice to find the chunk for
	 * @return Chunk which contains the data for the specified triangle
	 */
	FVCOMStructure::ChunkInfo getChunkForTriangle(int triangle, int siglay, int time) const;

	const std::vector<unsigned int>& getNodesInChunk(FVCOMStructure::ChunkInfo chunk) const;
	const std::vector<unsigned int>& getTrianglesInChunk(FVCOMStructure::ChunkInfo chunk) const;

	const std::vector<ModelFile> getModelFiles() const;

	const bool pointInModel(Point p, double time);
	const bool timeInModel(double time) const;
	const bool depthInModel(Point p);
	const bool xyInModel(Point p) const;

	const int getNumSiglays() const;

	/**
	 * Gets the index and percentage for linear interpolation of time
	 * @param time Time to interpolate with
	 * @param time1Index Output for the first time index for interpolation
	 * @param time2Index Output for the second time index for the interpolation
	 * @param time1Percent Output for the percent for time1Index for interpolation
	 */
	void timeInterpolation(double time, int& time1Index, int& time2Index, double& time1Percent) const;

	/**
	 * Gets the index and percentage for linear interpolation of time
	 * @param interpolatePoint Point to interpolate with
	 * @param siglay1Index Output for the first siglay index for interpolation
	 * @param siglay2Index Output for the second siglay index for the interpolation
	 * @param siglay1Percent Output for the percent for siglay1Index for interpolation
	 */
	void siglayInterpolation(FVCOMStructure::Point& interpolatePoint, int& siglay1Index, int& siglay2Index, double& siglay1Percent);
	void siglayInterpolation(FVCOMStructure::Point& interpolatePoint, int& siglay1Index, int& siglay2Index, double& siglay1Percent, int containingTriangle);


	/**
	* Gets the depth at a specific point.  Takes in a containingTriangle if avalible to reduce computation time.
	*@param interpolatePoint Point to get depth at
	*@param containingTriangle Containing triangle for this point
	*@param Depth at this point. Note this will be a positive number
	**/
	double getDepthAtPoint(FVCOMStructure::Point& interpolatePoint, int containingTriangle);

	/**
	* Gets the depth at a specific point.
	* @param interpolatePoint Point to get depth at
	* @param Depth at this point. Note this will be a positive number
	**/
	double getDepthAtPoint(FVCOMStructure::Point& interpolatePoint);

private:

	/**
	 * Loads all the data files
	 * @param filename Directory to load the files for
	 * @return List of filenames containing the model
	 */
	std::vector<std::string> traverseDataFiles(std::string filename);

	/**
	 * Helper function which loads all the model structure data from the model file
	 */
	void loadStructureData(const std::string directory);

	/**
	 * Helper function that determines the extent of the model
	 */
	void getModelExtent();

	/**
	 * Helper function which splits the model into chunks which can be individually loaded
	 */
	void splitIntoChunks();

private:

	/**
	 * File information for all the model files
	 */
	std::vector<ModelFile> modelFiles;

	/**
	 * Number of sigma layers
	 */
	unsigned int siglayDim;

	/**
	 * x,y for each node
	 */
	std::vector<Point> nodes;

	/**
	 * x,y for each triangle
	 */
	std::vector<Point> triangles;

	/**
	 * siglay for nodes
	 */
	std::vector<std::vector<float>> nodeSiglay;

	/**
	 * siglay for triangles
	 */
	std::vector<std::vector<float>> triangleSiglay;

	/**
	 * The times corresponding to each time index
	 */
	std::vector<float> times;

	/**
	 * List of nodes in each triangle
	 */
	std::vector<std::vector<int>> triangleToNodes;

	/**
	 * List of triangles that each node is a part of
	 */
	std::vector<std::vector<int>> nodeToTriangles;

	/**
	 * Chunk that a triangle is in
	 */
	std::vector<int> triangleToChunk;

	/**
	 * Chunk that a node is in
	 */
	std::vector<int> nodeToChunk;

	/**
	 * A list of all the nodes in each chunk for loading
	 */
	std::vector<std::vector<unsigned int>> nodesInChunk;

	/**
	 * A list of all the triangles in each chunk for loading
	 */
	std::vector<std::vector<unsigned int>> trianglesInChunk;


	// X,Y extent of the model
	double minX = std::numeric_limits<double>::max();
	double minY = std::numeric_limits<double>::max();;
	double maxX = -std::numeric_limits<double>::max();;
	double maxY = -std::numeric_limits<double>::max();;

	//Size of chunks in different dimensions
	unsigned int xChunkSize; //in meters
	unsigned int yChunkSize; //in meters
	unsigned int siglayChunkSize; //in siglay indices
	unsigned int timeChunkSize; //in time indicies

	//Number of chunks for each dimension
	unsigned int siglayDimChunks;
	unsigned int timeDimChunks;
	unsigned int yDimChunks;
	unsigned int xDimChunks;

	//Used in optimizations
	unsigned int lastContainingTriangle;
};

#endif