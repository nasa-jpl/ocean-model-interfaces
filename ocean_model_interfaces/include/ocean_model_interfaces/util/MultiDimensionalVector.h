#ifndef MULTI_DIMENSIONAL_VECTOR_H
#define MULTI_DIMENSIONAL_VECTOR_H

#include <vector>

namespace ocean_model_interfaces
{

template<typename T>
class MultiDimensionalVector {
public:
    MultiDimensionalVector<T>::MultiDimensionalVector();
    MultiDimensionalVector::MultiDimensionalVector(std::vector<int> dimensionSizes);

    T index(std::vector<unsigned int> indicies);

    T* getDataArray();
    T* getDataArrayAtIndex(std::vector<unsigned int> indicies);

    std::vector<int> size();

private:
    std::vector<T> data;
    std::vector<int> dimensionSizes;
};

}

#endif