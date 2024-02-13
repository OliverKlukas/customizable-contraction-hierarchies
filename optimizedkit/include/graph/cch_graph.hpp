#ifndef OPTIMIZEDKIT_CCH_GRAPH_HPP
#define OPTIMIZEDKIT_CCH_GRAPH_HPP

#include "customizable_contraction_hierarchy/cch_customizer.hpp"

namespace OptimizedKit {
    /**
     * @brief The CCH graph object storing the upwards graph extended representation and weights required for queries.
     *
     * @tparam WeightType - The type of the weights.
     */
    template<typename WeightType>
    class CchGraph {
    public:
        CchGraph() = default;

        /**
         * @brief Construct a CCH graph from a preprocessor and a customizer.
         *
         * @param preprocessor - The preprocessor object reference.
         * @param customizer - The customizer object reference.
         */
        CchGraph(const CchPreprocessor *preprocessor, const CchCustomizer<WeightType> *customizer);

        /**
         * @brief Construct a CCH graph from the upwards graph, upwards adjacency list and the forward and backward weights.
         *
         * @param upwardsGraph - The upwards graph.
         * @param forwardWeights - The forward weights.
         * @param backwardWeights - The backward weights.
         */
        CchGraph(const Graph *upwardsGraph, const std::vector<WeightType> *forwardWeights,
                 const std::vector<WeightType> *backwardWeights, unsigned long vertexCount);

    // private:
        const Graph *upwardsGraph{};
        const std::vector<WeightType> *forwardWeights;
        const std::vector<WeightType> *backwardWeights;
        unsigned long vertexCount{};
    };
}

#include "../../src/graph/cch_graph.tpp"

#endif //OPTIMIZEDKIT_CCH_GRAPH_HPP
