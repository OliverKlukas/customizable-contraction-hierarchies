#ifndef OPTIMIZEDKIT_NESTED_DISSECTION_INERTIAL_FLOW_ORDER_HPP
#define OPTIMIZEDKIT_NESTED_DISSECTION_INERTIAL_FLOW_ORDER_HPP

#include "utils/types.hpp"
#include "dinic_min_cut.hpp"
#include "graph/graph.hpp"
#include <cmath>

// TODO: refactor

namespace OptimizedKit {
    template<typename WeightType>
    class NestedDissectionInertialFlowOrder {
    public:
        struct GraphFragment {
            std::vector<VertexId> globalVertexIds;
            OptimizedKit::Graph graph;

            GraphFragment() = default;

            explicit GraphFragment(OptimizedKit::Graph graph) : graph(graph) {
                auto vertices = this->graph.getVertices();
                globalVertexIds.reserve(vertices->size());
                for (auto &vertex: *vertices) {
                    globalVertexIds.push_back(vertex.id);
                }
            }
        };

        struct SeparatorDecomposition {
            struct TreeNode {
                unsigned leftChild;
                unsigned rightSibling;
                unsigned FirstSeparatorVertex;
                unsigned lastSeparatorVertex;
            };

            std::vector<TreeNode> tree;
            std::vector<unsigned> order;
        };

        NestedDissectionInertialFlowOrder() = default;

        /**
         * @details Computes a nested dissection order with inertial flow of vertices in the graphFragment.
         *
         * @param graphFragment - GraphFragment to be used for computing the order.
         * @return Returns a vector of ordered vertices.
         */
        Order run(GraphFragment &graphFragment);

    private:
        Separator computeInertialFlowSeparator(Graph &graph);

        /**
         * @brief Sort vertices “spatially” by a linear combination of their coordinate’s latitude and longitude. Only
         *        the kth element is guaranteed to be in the correct position. All elements with index smaller than k
         *        are guaranteed to be smaller. Same applies for the the n-kth element and elements with index greater
         *        than n - k.
         *
         * @details Inspired by
         *          - https://github.com/Project-OSRM/osrm-backend/blob/886421b43aca2f6c06a10fe2d7705312fabb6b0f/src/partition/inertial_flow.cpp
         *          - https://daniel-j-h.github.io/post/selection-algorithms-for-partitioning/
         *
         * @param vertices - vertices to sort.
         * @param k - number of vertices to take from the beginning and the end.
         */
        void
        sortVerticesSpatiallyInPlace(std::vector<VertexId> &vertices,
                                     unsigned long k);

        /**
         * @brief Split the graph into two graphs based on the separator.
         *
         * @param graph - graph to split.
         * @param separator - separator to split the graph.
         * @return Returns a vector of two graphs.
         */
        std::vector<GraphFragment> splitGraphFragmentConnected(GraphFragment &graphFragment, Separator separator);

        void pickSmallerCutSide(OptimizedKit::DinicMinCut<WeightType>::MinCut &minCut);

    };


}

#include "../../src/graph_order_algorithms/nested_dissection_inertial_flow_order.tpp"

#endif //OPTIMIZEDKIT_NESTED_DISSECTION_INERTIAL_FLOW_ORDER_HPP
