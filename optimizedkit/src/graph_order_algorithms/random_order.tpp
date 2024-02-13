#include <graph_order_algorithms/random_order.hpp>

template<typename WeightType>
std::vector<OptimizedKit::VertexId>
OptimizedKit::RandomOrder<WeightType>::run(Graph &graph) {
    auto maxVertexId = std::max_element(graph.head.begin(), graph.head.end());
    std::vector<VertexId> vertexIds(*maxVertexId + 1);
    std::shuffle(vertexIds.begin(), vertexIds.end(), std::mt19937(std::random_device()()));
    return vertexIds;
}

template<typename WeightType>
OptimizedKit::RandomOrder<WeightType>::RandomOrder() {
    auto randomDevice = std::random_device{};
    generator = std::default_random_engine{randomDevice()};
}
