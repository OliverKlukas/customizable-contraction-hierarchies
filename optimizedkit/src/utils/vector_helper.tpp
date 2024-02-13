#include <utils/vector_helper.hpp>
#include "utils/constants.hpp"

template<typename T>
void OptimizedKit::printVectorToConsole(const std::vector<T> &vector, const std::string name) {
    std::cout << name << ": [";
    for (int i = 0; i < vector.size() - 1; ++i) {
        std::cout << vector[i] << ", ";
    }
    std::cout << vector[vector.size() - 1] << "]";
    std::cout << std::endl;
}

template<typename V>
std::vector<V> OptimizedKit::constructAdjacencyIndices(const std::vector<V> &v, unsigned edgeCount) {
    std::vector<unsigned> index(edgeCount + 1);
    assert(v.empty() || (OptimizedKit::isVectorSorted(v) && OptimizedKit::maxElementOfVector(v) < edgeCount));
    unsigned pos = 0;
    for (unsigned i = 0; i < edgeCount; ++i) {
        while (pos < v.size() && v[pos] < i)
            ++pos;
        index[i] = pos;
    }
    index[edgeCount] = v.size();
    return index;
}

template<typename t1, typename t2>
std::vector<t2> OptimizedKit::convertVector(const std::vector<t1> &vec) {
    static_assert(std::is_convertible<t1, t2>::value, "Types are not convertible");
    std::vector<t2> result;
    result.reserve(vec.size());
    for (const t1 &item: vec) {
        result.push_back(static_cast<t1>(item));
    }
    return result;
}

template<typename T>
void OptimizedKit::removeElementsByFilterInplace(std::vector<T> &vector, Filter filter) {
    assert(filter.size() == vector.size() && "Filter and vector mismatch");
    size_t index = 0;
    vector.erase(std::remove_if(vector.begin(), vector.end(), [&filter, &index](const T &) { return filter[index++]; }),
                 vector.end());
}

template<typename T>
void OptimizedKit::adjustElementsToRemoveFilterInPlace(std::vector<T> &vector, Filter filter) {
    for (unsigned currId = 0; currId < vector.size(); currId++) {
        // Check for invalid values (e.g. for loops in the input graph)
        if (vector[currId] == OptimizedKit::INVALID_VALUE<unsigned>)
            continue;

        // Check if cch edge is removed.
        assert(vector[currId] < filter.size() - 1);
        if (filter[vector[currId]]){
            vector[currId] = OptimizedKit::INVALID_VALUE<unsigned>;
            continue;
        }

        // Lower saved cch edge id by the number of removed edges for each lower id.
        T offset = 0;
        for (unsigned lowerId = 0; lowerId < vector[currId]; lowerId++) {
            if (filter[lowerId])
                offset++;
        }
        assert(vector[currId] >= offset);
        vector[currId] -= offset;
    }
}

template<typename T>
std::vector<T> readVectorFromBinaryFile(const std::string &filePath) {
    std::ifstream in(filePath, std::ios_base::binary);
    if (!in) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        throw std::runtime_error("File open failed");
    }
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    size_t numElements = size / sizeof(float);
    std::vector<T> data(numElements);
    in.read(reinterpret_cast<char *>(data.data()), size);
    in.close();
    return data;
}