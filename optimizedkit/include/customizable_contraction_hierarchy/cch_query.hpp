#ifndef OPTIMIZEDKIT_CCH_QUERY_HPP
#define OPTIMIZEDKIT_CCH_QUERY_HPP

#include <iostream>
#include <vector>
#include "cch_customizer.hpp"
#include "graph/graph.hpp"
#include "graph/cch_graph.hpp"
#include "utils/enums.hpp"
#include "utils/permutation.hpp"
#include "utils/constants.hpp"
#include "utils/vector_helper.hpp"
#include "utils/graph_helper.hpp"
#include "utils/id_mapper.hpp"
#include "path_finding_algorithms/bi_directional_dijkstra.hpp"
#include "customizable_contraction_hierarchy/cch_triangle_enumeration.hpp"

namespace OptimizedKit {
    template<typename WeightType>
    class CchQuery {
    public:
        explicit CchQuery(const CchCustomizer<WeightType> &customizer, HeapType heapType = HeapType::PAIRING);

        CchQuery<WeightType> &run(VertexId source, VertexId target, bool debug = false);

        std::vector<VertexId> getVertexPath();

        std::vector<EdgeId> getEdgePath();

        WeightType getQueryWeight();

        CchQuery<WeightType> &reset(const CchCustomizer<WeightType> &cchCustomizer);

        QueryState getState();

    // private:
        const CchCustomizer<WeightType> *cchCustomizer;
        const CchPreprocessor *cchPreprocessor;
        const CchGraph<WeightType> cchGraph;
        BiDirectionalDijkstra<WeightType> biDirectionalDijkstra;
        QueryState state{QueryState::UNINITIALIZED};

        std::vector<VertexId> vertexPath;
        std::vector<EdgeId> edgePath;

        VertexId globalSource{}, globalTarget{}, localSource{}, localTarget{};

        EdgeId unpackOriginalEdge(EdgeId cchEdge, bool forward);

        EdgeId unpackForwardOriginalEdge(EdgeId cchEdge);

        EdgeId unpackBackwardOriginalEdge(EdgeId cchEdge);

        template<class OnNewSegment>
        void unpackLowerTriangles(bool forward, VertexId x, VertexId y, EdgeId xy, const OnNewSegment &onMissingFound);
    };
}

#include "../../src/customizable_contraction_hierarchy/cch_query.tpp"

#endif //OPTIMIZEDKIT_CCH_QUERY_HPP
