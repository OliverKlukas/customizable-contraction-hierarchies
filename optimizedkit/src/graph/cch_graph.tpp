#include <graph/cch_graph.hpp>

template<typename WeightType>
OptimizedKit::CchGraph<WeightType>::CchGraph(const CchPreprocessor *preprocessor,
                                             const CchCustomizer <WeightType> *customizer) {
    upwardsGraph = &preprocessor->upwardsGraph;
    forwardWeights = &customizer->forwardWeights;
    backwardWeights = &customizer->backwardWeights;
    vertexCount = preprocessor->cchVertexCount();
}

template<typename WeightType>
OptimizedKit::CchGraph<WeightType>::CchGraph(const Graph *upwardsGraph,
                                             const std::vector<WeightType> *forwardWeights,
                                             const std::vector<WeightType> *backwardWeights,
                                             const unsigned long vertexCount) :  upwardsGraph(
        upwardsGraph), forwardWeights(forwardWeights), backwardWeights(
        backwardWeights), vertexCount(vertexCount) {}
