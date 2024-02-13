/* TODO: needs to be fixed
#include <gtest/gtest.h>
#include <unordered_set>
#include <cmath>
#include <random>
#include <graph_order_algorithms/dinic_min_cut.hpp>
#include "map/csv_reader.hpp"

TEST(DinicMinCutTest, run_munichGraphSourcesSinks_returnsMinCut) {
    // Arrange
    OptimizedKit::OldDirectedGraph<double> graph;
    auto testfile = "../../tests/test_data/munich.csv";
    OptimizedKit::CsvReader::unpackCsvDirected(testfile, graph);
    std::vector<OptimizedKit::OldDirectedGraph<double>::Vertex> vertices;
    vertices.reserve(graph.getVertices()->size());
    std::transform(graph.beginVertices(), graph.endVertices(), std::back_inserter(vertices),
                   [](const OptimizedKit::OldDirectedGraph<double>::Vertex &vertex) {
                       return OptimizedKit::OldDirectedGraph<double>::Vertex{vertex.id, vertex.latitude, vertex.longitude};
                   }
    );
    auto k = vertices.size() / 4;
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
    OptimizedKit::DinicMinCut<double> minCut;
    std::unordered_set<OptimizedKit::VertexId> sources;
    std::unordered_set<OptimizedKit::VertexId> sinks;
    for (auto i = 0; i < k; i++) {
        sources.insert(vertices[i].id);
        sinks.insert(vertices[vertices.size() - i - 1].id);
    }

    // Act
    OptimizedKit::DinicMinCut<double>::MinCut result = minCut.run(graph, sources, sinks);

    // Assert
    EXPECT_GE(result.num_vertices_source, sources.size());
    EXPECT_LE(result.num_vertices_source, graph.getVertexCount());
    EXPECT_GE(result.num_edges, 1);
    EXPECT_EQ(result.membershipFlags.size(), graph.getVertices()->size());
    EXPECT_EQ(result.num_vertices_source, 37936);
    EXPECT_EQ(result.num_edges, 51);

}

TEST(DinicMinCutTest, run_fakeGraphSourcesSink_returnsValidMinCut) {
    // Arrange
    OptimizedKit::OldDirectedGraph<double> graph;
    graph.addVertex(0, 0.0, 10.0);
    graph.addVertex(1, 0.0, 11.0);
    graph.addVertex(2, 0.0, 12.0);
    graph.addVertex(3, 0.0, 13.0);
    graph.addOrUpdateEdge(0, 1);
    graph.addOrUpdateEdge(1, 2);
    graph.addOrUpdateEdge(2, 3);
    graph.addOrUpdateEdge(3, 0);
    graph.addOrUpdateEdge(1, 3);

    graph.addVertex(4, 0.0, 40.0);
    graph.addVertex(5, 0.0, 41.0);
    graph.addVertex(6, 0.0, 42.0);
    graph.addVertex(7, 0.0, 43.0);
    graph.addOrUpdateEdge(4, 5);
    graph.addOrUpdateEdge(5, 6);
    graph.addOrUpdateEdge(6, 7);
    graph.addOrUpdateEdge(7, 4);
    graph.addOrUpdateEdge(5, 7);

    graph.addOrUpdateEdge(2, 5);

    std::vector<OptimizedKit::OldDirectedGraph<double>::Vertex> vertices;
    vertices.reserve(graph.getVertices()->size());
    std::transform(graph.beginVertices(), graph.endVertices(), std::back_inserter(vertices),
                   [](const OptimizedKit::OldDirectedGraph<double>::Vertex &vertex) {
                       return OptimizedKit::OldDirectedGraph<double>::Vertex{vertex.id, vertex.latitude, vertex.longitude};
                   }
    );
    auto k = vertices.size() / 4;
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
    OptimizedKit::DinicMinCut<double> dinicMinCut;
    std::unordered_set<OptimizedKit::VertexId> sources;
    std::unordered_set<OptimizedKit::VertexId> sinks;
    for (auto i = 0; i < k; i++) {
        sources.insert(vertices[i].id);
        sinks.insert(vertices[vertices.size() - i - 1].id);
    }

    // Act
    auto result = dinicMinCut.run(graph, sources, sinks);

    // Assert
    EXPECT_EQ(result.num_edges, 1);
    EXPECT_EQ(result.num_vertices_source, 4);
    EXPECT_EQ(result.membershipFlags.size(), 8);
    for (OptimizedKit::VertexId id = 0; id < result.membershipFlags.size(); id++) {
        if (id == 0 || id == 1 || id == 2 || id == 3) {
            EXPECT_TRUE(result.membershipFlags[id]);
        } else {
            EXPECT_FALSE(result.membershipFlags[id]);
        }
    }
}

TEST(DinicMinCutTest, run_SmallFakeGraphSourcesSink_returnsValidMinCut) {
    // Arrange
    OptimizedKit::OldDirectedGraph<double> graph;
    graph.addVertex(0, 0.0, 10.0);
    graph.addVertex(1, 0.0, 11.0);
    graph.addVertex(2, 0.0, 12.0);
    graph.addOrUpdateEdge(0, 1);
    graph.addOrUpdateEdge(1, 2);
    graph.addOrUpdateEdge(2, 0);

    std::vector<OptimizedKit::OldDirectedGraph<double>::Vertex> vertices;
    vertices.reserve(graph.getVertices()->size());
    std::transform(graph.beginVertices(), graph.endVertices(), std::back_inserter(vertices),
                   [](const OptimizedKit::OldDirectedGraph<double>::Vertex &vertex) {
                       return OptimizedKit::OldDirectedGraph<double>::Vertex{vertex.id, vertex.latitude, vertex.longitude};
                   }
    );
    auto k = ceil(static_cast<double>(vertices.size()) * 0.25);
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
    OptimizedKit::DinicMinCut<double> dinicMinCut;
    std::unordered_set<OptimizedKit::VertexId> sources;
    std::unordered_set<OptimizedKit::VertexId> sinks;
    for (auto i = 0; i < k; i++) {
        sources.insert(vertices[i].id);
        sinks.insert(vertices[vertices.size() - i - 1].id);
    }

    // Act
    auto result = dinicMinCut.run(graph, sources, sinks);

    // Assert
    EXPECT_EQ(result.num_edges, 2);
    EXPECT_GE(result.num_vertices_source, 1);
    EXPECT_EQ(result.membershipFlags.size(), 3);
    EXPECT_TRUE(result.membershipFlags[0] && !result.membershipFlags[1] && !result.membershipFlags[2] ||
                !result.membershipFlags[0] && result.membershipFlags[1] && !result.membershipFlags[2] ||
                !result.membershipFlags[0] && !result.membershipFlags[1] && result.membershipFlags[2]);
}
 */