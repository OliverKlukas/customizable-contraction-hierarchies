#ifndef OPTIMIZEDKIT_DINIC_MIN_CUT_HPP
#define OPTIMIZEDKIT_DINIC_MIN_CUT_HPP
#include <set>
#include <unordered_set>
#include <limits>
#include "utils/types.hpp"

// TODO: refactor

namespace OptimizedKit {
    /**
     * @brief Computes the minimum cut of a graph.
     *
     * @details The algorithm is based on Dinic's algorithm for maximum flow. All code in .hpp and .tpp is adapted from:
     *          https://github.com/Project-OSRM/osrm-backend/blob/886421b43aca2f6c06a10fe2d7705312fabb6b0f/include/partition/dinic_max_flow.hpp
     *
     * @tparam WeightType - Type of the distance of the edges.
     */
    template<typename WeightType>
    class DinicMinCut {
    public:
        using Level = std::uint32_t;
        using LevelGraph = std::vector<Level>;
        using FlowEdges = std::vector<std::set<VertexId>>;
        using SourceSinkVertices = std::unordered_set<VertexId>;
        using MinCut = struct MinCut {
            std::size_t num_vertices_source{};
            std::size_t num_edges{};
            std::vector<bool> membershipFlags;
        };

        /**
         * @brief Allocates a DinicMinCut object.
         */
        DinicMinCut() = default;

        /**
         * @brief Computes the minimum cut of a graph.
         *
         * @param graph - Graph to be used for computing the minimum cut.
         * @param source - Source vertex id.
         * @param sink - Sink vertex id.
         * @return Returns the minimum cut of the graph.
         */
        MinCut run(UndirectedGraph &graph, SourceSinkVertices &sources, SourceSinkVertices &sinks);

    private:
        /**
         * @brief Computes level graph of vertices in the residual graph based on hop distance to the source via BFS.
         *
         * @param graph - Graph to be used for computing the level graph.
         * @param border_source_vertices - Border source vertices.
         * @param source_vertices - Source vertices.
         * @param sink_vertices - Sink vertices.
         * @param flow - Flow edges.
         * @return Returns the level graph.
         */
        LevelGraph ComputeLevelGraph(UndirectedGraph &graph,
                                     std::vector<VertexId> &border_source_vertices,
                                     SourceSinkVertices &source_vertices,
                                     SourceSinkVertices &sink_vertices,
                                     FlowEdges &flow);

        /**
         * @brief Finds a blocking flow in the residual graph based on DFS on the level graph.
         *
         * @details A flow is Blocking Flow if no more flow can be sent using level graph, i.e., no more s-t path exists
         *          such that path vertices have current levels 0, 1, 2… in order. Blocking Flow can be seen same as
         *          maximum flow path in Greedy algorithm.
         *
         * @param flow - Flow edges.
         * @param levels - Level graph.
         * @param graph - OldDirectedGraph to be used for computing the blocking flow.
         * @param source_vertices - Source vertices.
         * @param border_sink_vertices - Border sink vertices.
         * @return Returns the flow value of the blocking flow.
         */
        std::size_t BlockingFlow(FlowEdges &flow,
                                 LevelGraph &levels,
                                 OldDirectedGraph<WeightType> &graph,
                                 SourceSinkVertices &source_vertices,
                                 std::vector<VertexId> &border_sink_vertices);

        /**
         * @brief Finds a single augmenting path from a vertix to the sink side following levels in the level graph.
         *
         * @param levels - Level graph.
         * @param vertexId - Starting vertex.
         * @param graph - OldDirectedGraph to be used for computing the augmenting path.
         * @param flow - Flow edges.
         * @param source_vertices - Source vertices.
         * @return Returns the augmenting path.
         */
        std::vector<VertexId> GetAugmentingPath(LevelGraph &levels,
                                                VertexId vertexId,
                                                OldDirectedGraph<WeightType> &graph,
                                                FlowEdges &flow,
                                                SourceSinkVertices &source_vertices);

        /**
         * @brief Computes the minimum cut of a graph based on the flow edges.
         *
         * @param graph - OldDirectedGraph to be used for computing the minimum cut.
         * @param levels - Level graph.
         * @param flow_value - Flow value.
         * @return Returns the minimum cut.
         */
        MinCut MakeCut(OldDirectedGraph<WeightType> &graph, LevelGraph &levels, std::size_t flow_value);

        /**
         * @brief Checks if a nvertex has neighbours that are not part of a set (i.e. source or sink set).
         *
         * @param set - Set of vertices.
         * @param graph - OldDirectedGraph reference.
         * @return Returns true if the vertex has neighbours that are not part of the set, false otherwise.
         */
        bool hasNeighboursNotInSet(typename OptimizedKit::DinicMinCut<WeightType>::SourceSinkVertices &set,
                                   OptimizedKit::OldDirectedGraph<WeightType> &graph,
                                   OptimizedKit::VertexId vertexId);
    };

}

#include "../../src/graph_order_algorithms/dinic_min_cut.tpp"

#endif //OPTIMIZEDKIT_DINIC_MIN_CUT_HPP
