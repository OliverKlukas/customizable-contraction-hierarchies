#include <graph_order_algorithms/nested_dissection_inertial_flow_order.hpp>
#include <graph_order_algorithms/dinic_min_cut.hpp>
/*
TODO: implementation to be used with undirected graphs and do the missing steps
template<typename WeightType>
OptimizedKit::Order OptimizedKit::NestedDissectionInertialFlowOrder<WeightType>::run(GraphFragment &graphFragment) {
    Order order;
    if (graphFragment.graph.getVertexCount() <= 1) {
        auto vertices = graphFragment.globalVertexIds;
        order.insert(order.end(), vertices.begin(), vertices.end());
        return order;
    }
    auto separator = computeInertialFlowSeparator(graphFragment.graph);
    auto fragments = splitGraphFragmentConnected(graphFragment, separator);
    for (auto &fragment: fragments) {
        auto suborder = run(fragment);
        order.insert(order.end(), suborder.begin(), suborder.end());
    }
    return order;
}

template<typename WeightType>
std::vector<typename OptimizedKit::NestedDissectionInertialFlowOrder<WeightType>::GraphFragment>
OptimizedKit::NestedDissectionInertialFlowOrder<WeightType>::splitGraphFragmentConnected(GraphFragment &graphFragment,
                                                                                         Separator separator) {
    assert(separator.size() == graphFragment.graph.getVertexCount());
    std::vector<GraphFragment> fragments(2);
    std::unordered_map<VertexId, VertexId> vertexIdToFragmentId;
    VertexId firstFragmentId = 0;
    VertexId secondFragmentId = 0;
    for (auto iterator = graphFragment.graph.beginVertices();
         iterator != graphFragment.graph.endVertices(); iterator++) {
        if (separator[iterator->id]) {
            fragments[0].graph.addVertex(firstFragmentId, iterator->latitude, iterator->longitude);
            fragments[0].globalVertexIds.push_back(graphFragment.globalVertexIds[iterator->id]);
            vertexIdToFragmentId[iterator->id] = firstFragmentId;
            firstFragmentId++;
        } else {
            fragments[1].graph.addVertex(secondFragmentId, iterator->latitude, iterator->longitude);
            fragments[1].globalVertexIds.push_back(graphFragment.globalVertexIds[iterator->id]);
            vertexIdToFragmentId[iterator->id] = secondFragmentId;
            secondFragmentId++;
        }
    }
    assert(firstFragmentId + secondFragmentId == graphFragment.graph.getVertexCount());
    for (auto vertexId = 0; vertexId < separator.size(); vertexId++) {
        auto outgoingEdges = graphFragment.graph.getOutgoingEdges(vertexId);
        for (auto &edge: *outgoingEdges) {
            if (separator[vertexId] && separator[edge.destination]) {
                fragments[0].graph.addOrUpdateEdge(vertexIdToFragmentId.at(vertexId),
                                                   vertexIdToFragmentId.at(edge.destination), edge.distance);
            } else if (!separator[vertexId] && !separator[edge.destination]) {
                fragments[1].graph.addOrUpdateEdge(vertexIdToFragmentId.at(vertexId),
                                                   vertexIdToFragmentId.at(edge.destination), edge.distance);
            }
        }
    }


    return fragments;
}

template<typename WeightType>
OptimizedKit::Separator OptimizedKit::NestedDissectionInertialFlowOrder<WeightType>::computeInertialFlowSeparator(
        OldDirectedGraph <WeightType> &graph) {
    auto vertices = *graph.getVertices();
    auto k = ceil(static_cast<double>(vertices.size()) * 0.25);
    sortVerticesSpatiallyInPlace(vertices, k);
    DinicMinCut <WeightType> dinicMinCut;

    std::unordered_set<VertexId> sources;
    std::unordered_set<VertexId> sinks;
    for (auto i = 0; i < k; i++) {
        sources.insert(vertices[i].id);
        sinks.insert(vertices[vertices.size() - i - 1].id);
    }

    // TODO: here one should do the balance check and find the best satisfying cut
    auto minCut = dinicMinCut.run(graph, sources, sinks);
    Separator separator = minCut.membershipFlags;
    return separator;
}

template<typename WeightType>
void OptimizedKit::NestedDissectionInertialFlowOrder<WeightType>::sortVerticesSpatiallyInPlace(
        std::vector<typename OptimizedKit::OldDirectedGraph<WeightType>::Vertex> &vertices, unsigned long k) {
    const double slope = 1;
    const auto spatialLinearCombination = [slope](const auto &vertex) {
        return slope * vertex.longitude + (1. - fabs(slope)) * vertex.latitude;
    };
    const auto spatialComparison = [&](const auto &lhs, const auto &rhs) {
        return spatialLinearCombination(lhs) < spatialLinearCombination(rhs);
    };
    const auto flip = [](auto fn) {
        return [fn](auto &&lhs, auto &&rhs) {
            return fn(std::forward<decltype(lhs)>(rhs), std::forward<decltype(rhs)>(lhs));
        };
    };
    nth_element(begin(vertices), begin(vertices) + k, end(vertices), spatialComparison);
    nth_element(rbegin(vertices), rbegin(vertices) + k, rend(vertices), flip(spatialComparison));
}

*/