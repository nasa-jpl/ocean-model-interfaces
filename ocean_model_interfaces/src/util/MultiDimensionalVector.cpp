#include "ocean_model_interfaces/util/MultiDimensionalVector.h"
#include <stdexcept>

using namespace ocean_model_interfaces;

template<typename T>
MultiDimensionalVector<T>::MultiDimensionalVector() {
}


template<typename T>
MultiDimensionalVector<T>::MultiDimensionalVector(std::vector<int> dimensionSizes) {
    this->dimensionSizes = dimensionSizes;

    int totalEntries = 1;
    for(int i = 0; i < dimensions.size(); i++) {
        totalEntries *= dimensions[i]
    }

    data.resize(totalEntries);
}

template<typename T>
T* MultiDimensionalVector<T>::getDataArray() {
    return data.data();
}

T* MultiDimensionalVector<T>::getDataArrayAtIndex(std::vector<unsigned int> indicies) {

}

template<typename T>
T MultiDimensionalVector<T>::index(std::vector<unsigned int> indicies) {
    if(indicies.size() != dimensionSizes.size()) {
        throw std::runtime_error("Number of provided indicies does not match the dimensions of the nD vector");
    }

    for(int i = 0; i < indicies.size(); i++) {
        if(indicies[i] >= dimensionSizes[i]) {
            throw std::runtime_error("MultiDimensionalVector index out of bounds.");
        }
    }

    int single_index = 0;
    int multiplier = 1;
    for(int i = indicies.size() - 1; i >= 0; i--) {
        single_index += multiplier * indicies[i];
        multiplier *= dimensionSizes[i]
    }

    return data[single_index];
}

template<typename T>
std::vector<int> MultiDimensionalVector<T>::size() {
    return dimensionSizes;
}


