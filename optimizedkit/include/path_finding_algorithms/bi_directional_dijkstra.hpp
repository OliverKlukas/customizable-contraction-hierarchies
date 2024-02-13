#ifndef OPTIMIZEDKIT_BI_DIRECTIONAL_DIJKSTRA_HPP
#define OPTIMIZEDKIT_BI_DIRECTIONAL_DIJKSTRA_HPP

#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <memory>
#include <queue>
#include "priority_queues/binary_min_heap.hpp"
#include "priority_queues/pairing_min_heap.hpp"
#include "utils/types.hpp"
#include "graph/cch_graph.hpp"
#include "utils/math.hpp"

namespace OptimizedKit {
    template<typename WeightType>
    class BiDirectionalDijkstra {
    public:
        BiDirectionalDijkstra() = default;
        explicit BiDirectionalDijkstra(const CchGraph<WeightType> &graph, HeapType heapType = HeapType::BINARY) :
                cchGraph(graph),
                source(INVALID_VALUE<VertexId>),
                target(INVALID_VALUE<VertexId>),
                meetingVertex(INVALID_VALUE<VertexId>),
                shortestPathLength(INFINITY_WEIGHT<WeightType>),
                vertexCount(graph.vertexCount) {
            switch (heapType) {
                case HeapType::BINARY:
                    forwardQueue = new BinaryMinHeap<WeightType, VertexId>();
                    backwardQueue = new BinaryMinHeap<WeightType, VertexId>();
                    break;
                case HeapType::PAIRING:
                    forwardQueue = new PairingMinHeap<WeightType, VertexId>();
                    backwardQueue = new PairingMinHeap<WeightType, VertexId>();
                    break;
                default:
                    throw std::invalid_argument("Invalid heap type.");
            }
        }
        BiDirectionalDijkstra &run(VertexId sourceId, VertexId targetId, bool debug = false);

        ~BiDirectionalDijkstra() {
            delete forwardQueue;
            delete backwardQueue;
        }

    // private:
        VertexId source{}, target{}, meetingVertex{};
        WeightType shortestPathLength;
        unsigned long vertexCount{};

        const CchGraph<WeightType> cchGraph;

        std::vector<WeightType> forwardDistance;
        std::vector<WeightType> backwardDistance;

        AbstractHeap<WeightType, VertexId>* forwardQueue;
        AbstractHeap<WeightType, VertexId>* backwardQueue;

        std::vector<VertexId> forwardPredecessor;
        std::vector<VertexId> backwardPredecessor;

        Filter forwardSettled;
        Filter backwardSettled;

        long long numEdgesExplored = 0;
        long long numVerticesExplored = 0;

        void initialize();
    };
}

#include "../../src/path_finding_algorithms/bi_directional_dijkstra.tpp"

#endif //OPTIMIZEDKIT_BI_DIRECTIONAL_DIJKSTRA_HPP
