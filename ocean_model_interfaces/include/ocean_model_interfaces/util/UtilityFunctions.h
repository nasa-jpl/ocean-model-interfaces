#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <vector>

static std::vector<std::string> traverseDataFiles(const std::string filename);

static void load3DData(std::vector<std::vector<std::vector<float>>> targetVector, std::vector<size_t> start, std::vector<size_t> count);

#endif