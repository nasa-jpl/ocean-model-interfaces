#ifndef FVCOM_CHUNK_H
#define FVCOM_CHUNK_H

#include <unordered_map>
#include <vector>

#include <netcdf>

#include "ocean_models/fvcom/FVCOMStructure.h"
namespace ocean_models
{

/**
 * Smallest section of on FVCOM model that can be loaded into momory at once.
 * This is used to subdivide the model as in most cases it will be too large to store
 * all of it in memory. Only used by the FVCOM and 
 */
class FVCOMChunk
{
    
public:
    struct NodeData
    {
        float temp;
        float salt;
        float dye;
    };

    struct TriangleData
    {
        float u;
        float v;
        float w;
    };

    struct NodeDataInterp
    {
        double temp;
        double salt;
        double dye;
    };

    struct TriangleDataInterp
    {
        double u;
        double v;
        double w;
    };

    typedef std::vector<FVCOMChunk::NodeData> NodeVector;
    typedef std::vector<FVCOMChunk::TriangleData> TriangleVector;

    /**
     * @param modelFiles File information for all files used by the model.
     * @param nodesToLoad List of nodes that are contained in this chunk
     * @param trianglesToLoad List of triangles that are contained in this chunk.
     * @param chunkInfo The chunk id and the location of the chunk in the larger model.
     */
    FVCOMChunk(const std::vector<FVCOMStructure::ModelFile> modelFiles, std::vector<unsigned int> nodesToLoad,
                                               std::vector<unsigned int> trianglesToLoad,
                                               FVCOMStructure::ChunkInfo chunkInfo);


    /**
     * @return Data at a specific node, siglay, and time index. All indcies are assumed to be valid for this chunk.
     */
    const FVCOMChunk::NodeData& getNodeData(const unsigned int node, const unsigned int siglay, const unsigned int time);

    /**
     * @return Data at a specific triangle, siglay, and time index. All indcies are assumed to be valid for this chunk.
     */
    const FVCOMChunk::TriangleData& getTriangleData(const unsigned int triangle, const unsigned int siglay, const unsigned int time);

private:
    /**
     * @return The index of the file that constains the specific time index.
     */
    const unsigned int getFileIndexForTimeIndex(const std::vector<FVCOMStructure::ModelFile>& modelFiles, const unsigned int timeIndex) const;

private:
    std::unordered_map<unsigned int, FVCOMChunk::NodeVector> nodes;
    std::unordered_map<unsigned int, FVCOMChunk::TriangleVector> triangles;

    const FVCOMStructure::ChunkInfo chunkInfo;
};

}
#endif
