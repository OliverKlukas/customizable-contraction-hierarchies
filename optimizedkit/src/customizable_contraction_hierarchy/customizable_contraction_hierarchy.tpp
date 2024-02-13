#include <customizable_contraction_hierarchy/customizable_contraction_hierarchy.hpp>
// TODO: update and perfect customization missing
template<typename WeightType>
OptimizedKit::CustomizableContractionHierarchy<WeightType>::CustomizableContractionHierarchy(
        const Order &order, const Graph &graph, const std::vector<WeightType> &weights) :
        CustomizableContractionHierarchy(order, graph, &weights[0]) {}

template<typename WeightType>
OptimizedKit::CustomizableContractionHierarchy<WeightType>::CustomizableContractionHierarchy(
        const Order &order, const Graph &graph, const WeightType *weights) : preprocessor(order, graph),
                                                                             customizer(preprocessor,
                                                                                                  weights),
                                                                             query(customizer.baseCustomization()) {}

template<typename WeightType>
OptimizedKit::CustomizableContractionHierarchy<WeightType> &
OptimizedKit::CustomizableContractionHierarchy<WeightType>::reset(const Order &order, const Graph &graph,
                                                                  const std::vector<WeightType> &weights) {
    return reset(order, graph, &weights[0]);
}

template<typename WeightType>
OptimizedKit::CustomizableContractionHierarchy<WeightType> &
OptimizedKit::CustomizableContractionHierarchy<WeightType>::reset(const Order &order, const Graph &graph,
                                                                  const WeightType *weights) {
    preprocessor = CchPreprocessor(order, graph);
    customizer.reset(preprocessor, weights);
    customizer.baseCustomization();
    query.reset(customizer);
    return *this;
}

template<typename WeightType>
OptimizedKit::CustomizableContractionHierarchy<WeightType> &
OptimizedKit::CustomizableContractionHierarchy<WeightType>::reset(const std::vector<WeightType> &weights) {
    customizer.reset(weights).baseCustomization();
    return *this;
}

template<typename WeightType>
OptimizedKit::CustomizableContractionHierarchy<WeightType> &
OptimizedKit::CustomizableContractionHierarchy<WeightType>::reset(const WeightType *weights) {
    customizer.reset(weights).baseCustomization();
    return *this;
}

template<typename WeightType>
OptimizedKit::CustomizableContractionHierarchy<WeightType> &
OptimizedKit::CustomizableContractionHierarchy<WeightType>::run(VertexId source, VertexId target) {
    query.run(source, target);
    return *this;
}

template<typename WeightType>
WeightType OptimizedKit::CustomizableContractionHierarchy<WeightType>::getQueryWeight() {
    assert(query.getState() == QueryState::FINISHED);
    return query.getQueryWeight();
}

template<typename WeightType>
std::vector<OptimizedKit::EdgeId> OptimizedKit::CustomizableContractionHierarchy<WeightType>::getEdgePath() {
    assert(query.getState() == QueryState::FINISHED);
    return query.getEdgePath();
}

template<typename WeightType>
std::vector<OptimizedKit::VertexId> OptimizedKit::CustomizableContractionHierarchy<WeightType>::getVertexPath() {
    assert(query.getState() == QueryState::FINISHED);
    return query.getVertexPath();
}
