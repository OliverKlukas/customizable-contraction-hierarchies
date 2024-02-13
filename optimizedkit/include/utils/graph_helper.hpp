#ifndef OPTIMIZEDKIT_GRAPH_HELPER_HPP
#define OPTIMIZEDKIT_GRAPH_HELPER_HPP

#include <vector>
#include <cassert>
#include "types.hpp"
#include "utils/vector_helper.hpp"

namespace OptimizedKit{
    /**
     * @brief Finds the edge between two vertices x and y given a sorted head.
     *
     * @copyright Inspired by RoutingKit's find_arc_or_return_invalid_given_sorted_head in graph_util.h.
     *
     * @param adjacencyList - Adjacency list of the graph.
     * @param head - Head of the graph.
     * @param x - Source vertex.
     * @param y - Target vertex.
     * @return Returns the edge between x and y.
     */
    EdgeId findEdge(const std::vector<VertexId>&adjacencyList, const std::vector<VertexId>&head, VertexId x, VertexId y);

    /**
     * @brief Converts an edge path to a vertex path.
     *
     * @param tail - Tail of the graph.
     * @param head - Head of the graph.
     * @param edgePath - Edge path to be converted to a vertex path.
     * @return Returns the converted vertex path.
     */
    std::vector<VertexId> convertEdgePathToVertexPath(const std::vector<VertexId> &tail, const std::vector<VertexId> &head, std::vector<EdgeId> &edgePath);

    /**
     * @brief Converts a vertex path to an edge path.
     *
     * @param tail - Tail of the graph.
     * @param head - Head of the graph.
     * @param vertexPath - Vertex path to be converted to an edge path.
     * @return Returns the converted edge path.
     */
    std::vector<EdgeId> convertVertexPathToEdgePath(const std::vector<VertexId> &tail, const std::vector<VertexId> &head, std::vector<VertexId> &vertexPath);
}

#endif //OPTIMIZEDKIT_GRAPH_HELPER_HPP
