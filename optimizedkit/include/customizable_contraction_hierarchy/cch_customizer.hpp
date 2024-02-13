#ifndef OPTIMIZEDKIT_CCH_CUSTOMIZER_HPP
#define OPTIMIZEDKIT_CCH_CUSTOMIZER_HPP

#include <vector>
#include <iostream>
#include "cch_preprocessor.hpp"
#include "graph/graph.hpp"
#include "utils/enums.hpp"
#include "utils/permutation.hpp"
#include "utils/constants.hpp"
#include "utils/vector_helper.hpp"
#include "utils/id_mapper.hpp"
#include "utils/math.hpp"
#include "priority_queues/binary_min_heap.hpp"
#include "priority_queues/pairing_min_heap.hpp"
#include "cch_triangle_enumeration.hpp"
#include "customizable_contraction_hierarchy/cch_triangle_enumeration.hpp"

namespace OptimizedKit {
    template<typename WeightType>
    class CchCustomizer {
    public:
        CchCustomizer(CchPreprocessor &preprocessor, const std::vector<WeightType> &weights, HeapType heapType_ = HeapType::PAIRING, bool debug_ = false);

        CchCustomizer(CchPreprocessor &preprocessor, const WeightType *weights, HeapType heapType_ = HeapType::BINARY, bool debug_ = false);

        CchCustomizer &reset(CchPreprocessor &preprocessor, const std::vector<WeightType> &weights);

        CchCustomizer &reset(CchPreprocessor &preprocessor, const WeightType *weights);

        CchCustomizer &reset(const std::vector<WeightType> &weights);

        CchCustomizer &reset(const WeightType *weights);

        CchCustomizer &update(const std::vector<unsigned> &updateIds);

        CchCustomizer &baseCustomization();

        CchCustomizer &perfectCustomization();

        std::vector<WeightType> forwardWeights;
        std::vector<WeightType> backwardWeights;
        const WeightType *inputWeights;
        CchPreprocessor *cchPreprocessor{};
        long long numChangedWeights = 0;
    private:
        void extractEdgeWeight(EdgeId edge);

        void extractRespectingMetric();

        void relaxLowerTriangle(EdgeId ab, EdgeId ac, EdgeId bc, VertexId a, VertexId b, VertexId c);

        CustomizerState state;

        HeapType heapType;

        bool debug = false;
    };


}

#include "../../src/customizable_contraction_hierarchy/cch_customizer.tpp"

#endif //OPTIMIZEDKIT_CCH_CUSTOMIZER_HPP
