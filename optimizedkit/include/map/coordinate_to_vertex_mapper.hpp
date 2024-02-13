#ifndef OPTIMIZEDKIT_COORDINATE_TO_VERTEX_MAPPER_HPP
#define OPTIMIZEDKIT_COORDINATE_TO_VERTEX_MAPPER_HPP

#include <vector>
#include <valarray>
#include <cassert>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>
#include "utils/types.hpp"
#include "utils/constants.hpp"

namespace OptimizedKit {
    class CoordinateToVertexMapper {
    public:
        CoordinateToVertexMapper(const std::vector<float> &latitudes, const std::vector<float> &longitudes) {
            assert(latitudes.size() == longitudes.size() && "Latitudes and longitudes must be of equal size.");
            assert(!latitudes.empty() && !longitudes.empty() && "Latitudes and longitudes must not be empty.");
            root = buildVpTree(latitudes, longitudes);
            originalLatitudes = latitudes;
            originalLongitudes = longitudes;
        }

        [[nodiscard]] VertexId findNearestVertex(float latitude, float longitude, unsigned radius = 250) const;

        [[nodiscard]] double
        haversineDistance(float latitude1, float longitude1, float latitude2, float longitude2) const;

    private:
        struct VpTreeNode {
            float vantageLat;
            float vantageLon;
            double threshold;
            VpTreeNode *innerSubset;
            VpTreeNode *outerSubset;
        };

        VpTreeNode *buildVpTree(const std::vector<float> &latitudes, const std::vector<float> &longitudes);

        std::pair<float, float>
        nearestNeighbor(float latitude, float longitude, VpTreeNode *node, std::pair<float, float> bestNeighbor,
                        double bestDistance, unsigned int radius) const;

        double EARTH_RADIUS = 6371000.0;
        VpTreeNode *root;
        std::vector<float> originalLatitudes;
        std::vector<float> originalLongitudes;
    };
}

#endif //OPTIMIZEDKIT_COORDINATE_TO_VERTEX_MAPPER_HPP
