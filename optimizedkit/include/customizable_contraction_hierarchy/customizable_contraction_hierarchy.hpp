#ifndef OPTIMIZEDKIT_CUSTOMIZABLE_CONTRACTION_HIERARCHY_HPP
#define OPTIMIZEDKIT_CUSTOMIZABLE_CONTRACTION_HIERARCHY_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include "cch_preprocessor.hpp"
#include "cch_query.hpp"
#include "cch_customizer.hpp"

namespace OptimizedKit {
    template <typename WeightType>
    class CustomizableContractionHierarchy {
    public:
        CustomizableContractionHierarchy(const Order &order, const Graph &graph,
                                         const std::vector<WeightType> &weights);

        CustomizableContractionHierarchy(const Order &order, const Graph &graph, const WeightType *weights);

        CustomizableContractionHierarchy& reset(const Order &order, const Graph &graph,
                                                const std::vector<WeightType> &weights);

        CustomizableContractionHierarchy& reset(const Order &order, const Graph &graph,
                                                const WeightType *weights);

        CustomizableContractionHierarchy& reset(const std::vector<WeightType> &weights);

        CustomizableContractionHierarchy& reset(const WeightType *weights);

        CustomizableContractionHierarchy& run(VertexId source, VertexId target);

        std::vector<VertexId> getVertexPath();

        std::vector<EdgeId> getEdgePath();

        WeightType getQueryWeight();

    private:
        CchPreprocessor preprocessor;
        CchCustomizer<WeightType> customizer;
        CchQuery<WeightType> query;
    };
}

#include "../../src/customizable_contraction_hierarchy/customizable_contraction_hierarchy.tpp"

#endif //OPTIMIZEDKIT_CUSTOMIZABLE_CONTRACTION_HIERARCHY_HPP
