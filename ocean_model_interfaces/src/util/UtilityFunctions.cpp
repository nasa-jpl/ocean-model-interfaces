#include "ocean_model_interfaces/util/UtilityFunctions.h"
#include <boost/filesystem.hpp>

using namespace ocean_model_interfaces;

static std::vector<std::string> traverseDataFiles(const std::string filename)
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