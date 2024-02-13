#include "map/coordinate_to_vertex_mapper.hpp"

using namespace OptimizedKit;

#include <random>

CoordinateToVertexMapper::VpTreeNode *
CoordinateToVertexMapper::buildVpTree(const std::vector<float> &latitudes, const std::vector<float> &longitudes) {
    // Base case: If there's only one point, create a leaf node.
    if (latitudes.empty()) {
        return nullptr;
    } else if (latitudes.size() == 1) {
        auto *leafNode = new VpTreeNode;
        leafNode->vantageLat = latitudes[0];
        leafNode->vantageLon = longitudes[0];
        leafNode->threshold = 0.0;
        leafNode->innerSubset = nullptr;
        leafNode->outerSubset = nullptr;
        return leafNode;
    }

    // Calculate the VP (choose the first point).
    auto *currentNode = new VpTreeNode;
    currentNode->vantageLat = latitudes[0];
    currentNode->vantageLon = longitudes[0];

    // Calculate distances from VP to all other points and store them in a vector.
    std::vector<double> distances;
    for (size_t i = 0; i < latitudes.size(); ++i) {
        if (i == 0) continue; // Skip the VP itself.
        double dist = haversineDistance(latitudes[0], longitudes[0], latitudes[i], longitudes[i]);
        distances.push_back(dist);
    }

    // Choose a random threshold from the distances vector.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distIndex(0, distances.size() - 1);
    size_t randomIndex = distIndex(gen);
    double randomThreshold = distances[randomIndex];

    // Set the threshold for the current node.
    currentNode->threshold = randomThreshold;

    // Divide the dataset into inner and outer subsets.
    std::vector<float> innerLatitudes;
    std::vector<float> innerLongitudes;
    std::vector<float> outerLatitudes;
    std::vector<float> outerLongitudes;

    for (size_t i = 1; i < latitudes.size(); ++i) {
        if (distances[i - 1] <= randomThreshold) {
            innerLatitudes.push_back(latitudes[i]);
            innerLongitudes.push_back(longitudes[i]);
        } else {
            outerLatitudes.push_back(latitudes[i]);
            outerLongitudes.push_back(longitudes[i]);
        }
    }

    // Recursively build VP Trees for inner and outer subsets.
    currentNode->innerSubset = buildVpTree(innerLatitudes, innerLongitudes);
    currentNode->outerSubset = buildVpTree(outerLatitudes, outerLongitudes);

    return currentNode;
}

std::pair<float, float> CoordinateToVertexMapper::nearestNeighbor(float latitude, float longitude, VpTreeNode *node,
                                                                  std::pair<float, float> bestNeighbor,
                                                                  double bestDistance, unsigned int radius) const {
    if (node == nullptr) {
        return bestNeighbor;
    }

    double dist = haversineDistance(latitude, longitude, node->vantageLat, node->vantageLon);
    // std::cout << "Debug: Distance to VP (" << node->vantageLat << ", " << node->vantageLon << "): " << dist << std::endl;

    if (dist < bestDistance && std::abs(dist) <= radius) {
        bestDistance = dist;
        bestNeighbor = {node->vantageLat, node->vantageLon};
    }

    if (dist - bestDistance <= node->threshold + radius) {
        // std::cout << "Debug: Exploring inner subset." << std::endl;
        bestNeighbor = nearestNeighbor(latitude, longitude, node->innerSubset, bestNeighbor, bestDistance, radius);
    }

    if (dist + bestDistance >= node->threshold - radius) {
        // std::cout << "Debug: Exploring outer subset." << std::endl;
        bestNeighbor = nearestNeighbor(latitude, longitude, node->outerSubset, bestNeighbor, bestDistance, radius);
    }

    return bestNeighbor;
}

VertexId CoordinateToVertexMapper::findNearestVertex(float latitude, float longitude, unsigned int radius) const {
    auto bestNeighbour = nearestNeighbor(latitude, longitude, root,
                                         {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
                                         std::numeric_limits<double>::max(), radius);
    auto bestLat = bestNeighbour.first;
    auto bestLon = bestNeighbour.second;
    std::cout << "Debug: Best neighbor: (" << bestLat << ", " << bestLon << ")" << std::endl;
    if (bestLat == std::numeric_limits<float>::max() && bestLon == std::numeric_limits<float>::max()) {
        std::cerr << "No nearest neighbor found within the given radius." << std::endl;
        return INVALID_VALUE<VertexId>;
    }
    auto it = std::find(originalLatitudes.begin(), originalLatitudes.end(), bestLat);
    assert(it != originalLatitudes.end() && "Latitude not found in original dataset.");
    auto index = std::distance(originalLatitudes.begin(), it);
    assert(originalLongitudes[index] == bestLon && "Longitude does not match latitude.");
    return static_cast<VertexId>(index);
}

double CoordinateToVertexMapper::haversineDistance(float latitude1, float longitude1, float latitude2,
                                                   float longitude2) const {
    auto deltaLat = (latitude2 - latitude1) * M_PI / 180.0;
    auto deltaLon = (longitude2 - longitude1) * M_PI / 180.0;

    auto radLat1 = latitude1 * M_PI / 180.0;
    auto radLat2 = latitude2 * M_PI / 180.0;

    double a = pow(sin(deltaLat / 2), 2) +
               pow(sin(deltaLon / 2), 2) *
               cos(radLat1) * cos(radLat2);
    double c = 2 * asin(sqrt(a));
    return EARTH_RADIUS * c;
}
