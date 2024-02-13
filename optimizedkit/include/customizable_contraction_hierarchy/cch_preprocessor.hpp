#ifndef OPTIMIZEDKIT_CCH_PREPROCESSOR_HPP
#define OPTIMIZEDKIT_CCH_PREPROCESSOR_HPP

#include <iostream>
#include <vector>
#include "graph/graph.hpp"
#include "utils/enums.hpp"
#include "utils/permutation.hpp"
#include "utils/constants.hpp"
#include "utils/vector_helper.hpp"
#include "utils/id_mapper.hpp"
#include "utils/math.hpp"

namespace OptimizedKit {
    class CchPreprocessor {
    public:
        CchPreprocessor(OptimizedKit::Order inputOrder, OptimizedKit::Graph graph);

        [[nodiscard]] unsigned long cchVertexCount() const { return rank.size(); }

        [[nodiscard]] unsigned cchEdgeCount() const { return upwardsGraph.getEdgeCount(); }

        [[nodiscard]] unsigned long inputVertexCount() const { return order.size(); }

        void removeEdges(const Filter &removeEdgeFilter);

        // Input variables.
        Order order;
        Graph inputGraph;
        std::vector<VertexId> rank;
        std::vector<VertexId> inputEdgeIds;

        // Input to cch mapping.
        VertexMapping inputEdgeToCchEdge;
        VertexMapping cchEdgeToInputEdge;
        Filter isInputEdgeUpwards;

        // Contracted upwards ordered graph for customization and query.
        Graph upwardsGraph;

        // Triangle Enumeration Helper.
        VertexMapping downwardsToUpwardsGraph;
        Graph downwardsGraph;

        // Helper to unpack cch edges to input edges.
        Filter doesCchEdgeHaveInputEdge;
        IdMapper doesCchEdgeHaveInputEdgeMapper;
        std::vector<EdgeId> forwardInputEdgeOfCchEdge;
        std::vector<EdgeId> backwardInputEdgeOfCchEdge;
        Filter doesCchEdgeHaveExtraInputEdge;
        IdMapper doesCchEdgeHaveExtraInputEdgeMapper;
        std::vector<EdgeId> extraForwardInputEdgeOfCchAdjacencyEdges;
        std::vector<EdgeId> extraBackwardInputEdgeOfCchAdjacencyEdges;
        std::vector<EdgeId> extraForwardInputEdgeOfCch;
        std::vector<EdgeId> extraBackwardInputEdgeOfCch;
    private:
        void applyOrder();

        void sortGraph();

        void buildUpwardsGraph();

        void buildInputToCchMapping();

        void buildDownwardsGraph();

        void buildCchToInputMapping();
    };
}

#endif //OPTIMIZEDKIT_CCH_PREPROCESSOR_HPP

