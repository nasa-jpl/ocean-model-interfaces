#ifndef MULTI_DIMENSIONAL_VECTOR_H
#define MULTI_DIMENSIONAL_VECTOR_H

#include <vector>
#include <stdexcept>

namespace ocean_model_interfaces
{

template<typename T>
class MultiDimensionalVector {
public:
    MultiDimensionalVector() {}

    MultiDimensionalVector(std::vector<size_t> dimensionSizes) {
        this->dimensionSizes = dimensionSizes;

        int totalEntries = 1;
        for(int i = 0; i < dimensionSizes.size(); i++) {
            totalEntries *= dimensionSizes[i];
        }

        data.resize(totalEntries);
    }

    T index(std::vector<size_t> indicies)  {
        if(indicies.size() != dimensionSizes.size()) {
            throw std::runtime_error("Number of provided indicies does not match the dimensions of the nD vector");
        }

        for(int i = 0; i < indicies.size(); i++) {
            if(0 > indicies[i] || indicies[i] >= dimensionSizes[i]) {
                throw std::runtime_error("MultiDimensionalVector index out of bounds.");
            }
        }

        size_t single_index = getFlattenedIndex(indicies);

        return data[single_index];
    }

    T* getDataArray()  {
        return data.data();
    }

    T* getDataArrayAtIndex(std::vector<size_t> indicies) {
        size_t index = getFlattenedIndex(indicies);
        return (data.data() + index);
    }

    std::vector<size_t> size()  {
        return dimensionSizes;
    }

private:
    size_t getFlattenedIndex(std::vector<size_t> indicies) {
        int single_index = 0;
        int multiplier = 1;
        for(int i = indicies.size() - 1; i >= 0; i--) {
            single_index += multiplier * indicies[i];
            multiplier *= dimensionSizes[i];
        }

        return single_index;
    }

private:
    std::vector<T> data;
    std::vector<size_t> dimensionSizes;
};

}
#endif