/* TODO: fix my nested dissection order
#include <gtest/gtest.h>
#include "map/csv_reader.hpp"
#include <graph_order_algorithms/nested_dissection_order.hpp>
#include "routingkit/nested_dissection.h"
#include "graph/undirected_graph.hpp"
TEST(NestedDissectionOrderTest, Run_FakeGraph_ReturnsSameOrderAsRoutingKit) {
    // Arrange
    OptimizedKit::OldDirectedGraph<double> optimizedKitGraph;
    optimizedKitGraph.addVertex(0, 0.0, 10.0);
    optimizedKitGraph.addVertex(1, 0.0, 11.0);
    optimizedKitGraph.addVertex(2, 0.0, 12.0);
    optimizedKitGraph.addVertex(3, 0.0, 13.0);
    optimizedKitGraph.addOrUpdateEdge(0, 1);
    optimizedKitGraph.addOrUpdateEdge(1, 2);
    optimizedKitGraph.addOrUpdateEdge(2, 3);
    optimizedKitGraph.addOrUpdateEdge(3, 0);
    optimizedKitGraph.addOrUpdateEdge(1, 3);
    optimizedKitGraph.addVertex(4, 0.0, 40.0);
    optimizedKitGraph.addVertex(5, 0.0, 41.0);
    optimizedKitGraph.addVertex(6, 0.0, 42.0);
    optimizedKitGraph.addVertex(7, 0.0, 43.0);
    optimizedKitGraph.addOrUpdateEdge(4, 5);
    optimizedKitGraph.addOrUpdateEdge(5, 6);
    optimizedKitGraph.addOrUpdateEdge(6, 7);
    optimizedKitGraph.addOrUpdateEdge(7, 4);
    optimizedKitGraph.addOrUpdateEdge(5, 7);
    optimizedKitGraph.addOrUpdateEdge(2, 5);
    auto optimizedKitGraphFragment = OptimizedKit::NestedDissectionInertialFlowOrder<double>::GraphFragment(
            optimizedKitGraph);

    OptimizedKit::UndirectedGraph routingKitGraph;
    routingKitGraph.VertexCount = 8;
    routingKitGraph.tail = {0, 1, 2, 3, 1, 4, 5, 6, 7, 5, 2};
    routingKitGraph.head = {1, 2, 3, 0, 3, 5, 6, 7, 4, 7, 5};
    std::vector<float> longitudes = {10.0, 11.0, 12.0, 13.0, 40.0, 41.0, 42.0, 43.0};
    std::vector<float> latitudes = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    // Act
    assert(optimizedKitGraph.getVertexCount() == routingKitGraph.VertexCount);
    auto routingKitOrder = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(
            routingKitGraph.VertexCount,
            routingKitGraph.tail,
            routingKitGraph.head,
            latitudes,
            longitudes,
            [](const std::string &message) {
                std::cout << message << std::endl;
            });
    auto optimizedKitOrder = OptimizedKit::NestedDissectionInertialFlowOrder<double>().run(optimizedKitGraphFragment);

    // Assert
    EXPECT_EQ(optimizedKitOrder.size(), optimizedKitGraph.getVertexCount());
    EXPECT_EQ(routingKitOrder.size(), optimizedKitOrder.size());
    auto differentElements = 0;
    for (auto i = 0; i < routingKitOrder.size(); i++) {
        if (routingKitOrder[i] != optimizedKitOrder[i]) {
            differentElements++;
        }
    }
    EXPECT_EQ(differentElements, 0);
}*/
