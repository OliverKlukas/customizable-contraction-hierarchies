#ifndef OPTIMIZEDKIT_DIJKSTRA_HPP
#define OPTIMIZEDKIT_DIJKSTRA_HPP

#include "utils/constants.hpp"
#include "utils/types.hpp"
#include "graph/cch_graph.hpp"
#include "priority_queues/binary_min_heap.hpp"

namespace OptimizedKit {
    template<typename WeightType = double>
    class Dijkstra {
    public:
        Dijkstra() = default;
        explicit Dijkstra(const CchGraph<WeightType> &graph) :
                cchGraph(graph),
                source(INVALID_VALUE<VertexId>),
                vertexCount(graph.upwardsGraph->vertexCount) {}

        Dijkstra &initialize(VertexId sourceId, bool debug = false);
        WeightType getQueryWeight(VertexId target, bool debug = false);
        std::vector<EdgeId> getEdgePath(VertexId target, bool debug = false);
        std::vector<VertexId> getVertexPath(VertexId target, bool debug = false);

    // private:
        VertexId source{};
        const CchGraph<WeightType> cchGraph;
        BinaryMinHeap<WeightType, VertexId> queue;
        std::vector<VertexId> predecessors;
        std::vector<WeightType> distances;
        unsigned vertexCount{};

        void ensureCorrectInitialization();
    };
}

#include "../src/path_finding_algorithms/dijkstra.tpp"

#endif //OPTIMIZEDKIT_DIJKSTRA_HPP
