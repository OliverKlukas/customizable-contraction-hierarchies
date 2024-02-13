#include <stdexcept>
#include <graph/graph.hpp>
#include "utils/constants.hpp"
#include "utils/vector_helper.hpp"

void OptimizedKit::Graph::addEdge(VertexId tailId, VertexId headId) {
    tail.push_back(tailId);
    head.push_back(headId);
}

OptimizedKit::EdgeId
OptimizedKit::Graph::getEdgeId(VertexId tailId, VertexId headId) const {
    for (EdgeId arcId = 0; arcId < getEdgeCount(); arcId++) {
        if (tail[arcId] == tailId && head[arcId] == headId)
            return arcId;
        if (tail[arcId] == headId && head[arcId] == tailId)
            return arcId;
    }
    return INVALID_VALUE<EdgeId>;
}

OptimizedKit::Graph::Graph(const OptimizedKit::Graph & other) {
    std::copy(other.tail.begin(), other.tail.end(), std::back_inserter(tail));
    std::copy(other.head.begin(), other.head.end(), std::back_inserter(head));
    std::copy(other.adjacencyIndices.begin(), other.adjacencyIndices.end(), std::back_inserter(adjacencyIndices));
    vertexCount = other.vertexCount;
}

void OptimizedKit::Graph::createAdjacencyIndices() {
    adjacencyIndices = constructAdjacencyIndices(tail, vertexCount);
}

void OptimizedKit::Graph::removeEdges(const OptimizedKit::Filter &removeEdgeFilter) {
    removeElementsByFilterInplace(tail, removeEdgeFilter);
    removeElementsByFilterInplace(head, removeEdgeFilter);
    createAdjacencyIndices();
}

