#ifndef OPTIMIZEDKIT_RANDOM_ORDER_HPP
#define OPTIMIZEDKIT_RANDOM_ORDER_HPP

#include <random>
#include "graph/graph.hpp"
#include "utils/types.hpp"

namespace OptimizedKit {
    template<typename WeightType>
    class RandomOrder {
    public:
        /**
         * @brief Allocates a random generator for graph orders.
         */
        explicit RandomOrder();

/**
         * @details Computes a random order of vertices in the graph.
         *
         * @param graph - Graph to be used for computing the order.
         * @return Returns a vector of vertices in random order.
         */
        std::vector<VertexId> run(Graph &graph);

    private:
        std::default_random_engine generator;
    };
}

#include "../../src/graph_order_algorithms/random_order.tpp"

#endif //OPTIMIZEDKIT_RANDOM_ORDER_HPP
