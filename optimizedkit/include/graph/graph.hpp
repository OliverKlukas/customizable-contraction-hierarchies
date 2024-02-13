#ifndef OPTIMIZEDKIT_GRAPH_HPP
#define OPTIMIZEDKIT_GRAPH_HPP

#include <vector>
#include "utils/types.hpp"
#include <cassert>

namespace OptimizedKit {
    class Graph {
    public:
        Graph() = default;

        Graph(const Graph& other);

        void addEdge(VertexId tailId, VertexId headId);

        [[nodiscard]] EdgeId getEdgeId(VertexId tailId, VertexId headId) const;

        [[nodiscard]] unsigned getEdgeCount() const { return tail.size(); }

        void createAdjacencyIndices();

        void removeEdges(const Filter &removeEdgeFilter);

    // private:
        std::vector<VertexId> tail;
        std::vector<VertexId> head;
        std::vector<EdgeId> adjacencyIndices;
        unsigned vertexCount;
    };
}

#endif //OPTIMIZEDKIT_GRAPH_HPP
