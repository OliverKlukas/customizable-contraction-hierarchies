#ifndef OPTIMIZEDKIT_VECTOR_HELPER_HPP
#define OPTIMIZEDKIT_VECTOR_HELPER_HPP

#include <vector>
#include <functional>
#include <cassert>
#include <fstream>
#include <iostream>
#include "utils/types.hpp"

namespace OptimizedKit {
    /**
     * @brief Checks if a vector is sorted using std::less.
     * @tparam V - Type of the vector.
     * @param v - Vector to check.
     * @return Returns true if the vector is sorted, false otherwise.
     */
    template<typename V>
    bool isVectorSorted(const std::vector<V> &v) {
        return std::is_sorted(v.begin(), v.end(), std::less<>());
    }

    /**
     * @brief Computes the maximum element of a vector.
     *
     * @tparam V - Type of the vector.
     * @param v - Vector to compute maximum element of.
     * @return Returns the maximum element of the vector.
     */
    template<typename V>
    unsigned maxElementOfVector(const std::vector<V> &v) {
        assert(!v.empty());
        return *std::max_element(v.begin(), v.end());
    }

    /**
     * @brief Computes the adjacency indices of a graph.
     *
     * @detail Inspired by the implementation of RoutingKit::invert_vector.
     *
     * @tparam V - Type of the vector.
     * @param v - Vector to compute inverse of.
     * @param edgeCount - Number of elements in the vector.
     * @return Returns the inverse vector with the element count as the last element.
     */
    template<typename V>
    std::vector<V> constructAdjacencyIndices(const std::vector<V> &v, unsigned edgeCount);

    /**
     * @brief Prints a vector to the console.
     *
     * @tparam V - Type of the vector.
     * @param vector - Vector to print.
     * @param name - Name of the vector to print.
     */
    template<typename V>
    void printVectorToConsole(const std::vector<V> &vector, std::string name);

    /**
     * @brief Converts a vector of type t1 to a vector of type t2 as a copy.
     *
     * @tparam t1 - Type of the vector to convert.
     * @tparam t2 - Type of the vector to convert to.
     * @param vec - Vector to convert.
     * @return Returns the converted vector as a copy of the original.
     */
    template<typename t1, typename t2>
    std::vector<t2> convertVector(const std::vector<t1>& vec);

    /**
     * @brief Removes elements from the vector that were marked with true in the filter.
     *
     * @tparam T - Type of the vector.
     * @param vector - Vector to filter.
     * @param filter - Lamda predicate to apply.
     */
    template<typename T>
    void removeElementsByFilterInplace(std::vector<T> &vector, Filter filter);

    /**
     * @brief Adjusts ids of elements in the vector that were marked in the filter to be left out.
     *
     * @tparam T - Type of the vector.
     * @param vector - Vector to filter.
     * @param filter - Lamda predicate to apply.
     */
    template<typename T>
    void adjustElementsToRemoveFilterInPlace(std::vector<T> &vector, Filter filter);

    /**
     * @brief Reads a vector from a binary file.
     *
     * @tparam T - Type of the vector.
     * @param filePath - Path to the binary file.
     * @return Returns the vector read from the binary file.
     */
    template <typename T>
    std::vector<T> readVectorFromBinaryFile(const std::string& filePath);
}

#include "../../src/utils/vector_helper.tpp"

#endif //OPTIMIZEDKIT_VECTOR_HELPER_HPP
