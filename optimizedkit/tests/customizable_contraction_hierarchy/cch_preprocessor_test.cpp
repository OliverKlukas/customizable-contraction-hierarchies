#include <gtest/gtest.h>
#include "graph/graph.hpp"
#include "customizable_contraction_hierarchy/cch_preprocessor.hpp"
#include <routingkit/customizable_contraction_hierarchy.h>
#include "map/csv_reader.hpp"
#include "../test_utils/utils.hpp"
#include <routingkit/nested_dissection.h>

TEST(CchPreprocessorTest, Preprocess_WithOsmMap_ExpectSamePreprocessingAsRoutingKit) {
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<float> weights;
    auto testfile = "../test_data/munich.csv";
    auto startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);
    std::vector<unsigned> order = readVectorFromBinaryFile<unsigned>("../../../experiments/data/munich/roadway_graph/order");
    graph.vertexCount = latitudes.size();
    auto endTime = std::chrono::high_resolution_clock::now();
    printDuration("map & order prepared", startTime, endTime);

    // Act
    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch preprocessed", startTime, endTime);
    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch preprocessed", startTime, endTime);

    // Assert
    ASSERT_EQ(routingKitCch.order.size(), preprocessor.order.size()) << "Vectors routingKitCch.order and preprocessor.order are of unequal length";
    for (int i = 0; i < routingKitCch.order.size(); ++i) {
        EXPECT_EQ(routingKitCch.order[i], preprocessor.order[i]) << "Vectors routingKitCch.order and preprocessor.order differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.rank.size(), preprocessor.rank.size()) << "Vectors routingKitCch.rank and preprocessor.rank are of unequal length";
    for (int i = 0; i < routingKitCch.rank.size(); ++i) {
        EXPECT_EQ(routingKitCch.rank[i], preprocessor.rank[i]) << "Vectors routingKitCch.rank and preprocessor.rank differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.up_first_out.size(), preprocessor.upwardsGraph.adjacencyIndices.size()) << "Vectors routingKitCch.up_first_out and preprocessor.upwardsAdjacencyList are of unequal length";
    for (int i = 0; i < routingKitCch.up_first_out.size(); ++i) {
        EXPECT_EQ(routingKitCch.up_first_out[i], preprocessor.upwardsGraph.adjacencyIndices[i]) << "Vectors routingKitCch.up_first_out and preprocessor.upwardsAdjacencyList differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.up_head.size(), preprocessor.upwardsGraph.head.size()) << "Vectors routingKitCch.up_head and preprocessor.upwardsGraph.head are of unequal length";
    for (int i = 0; i < routingKitCch.up_head.size(); ++i) {
        EXPECT_EQ(routingKitCch.up_head[i], preprocessor.upwardsGraph.head[i]) << "Vectors routingKitCch.up_head and preprocessor.upwardsGraph.head differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.up_tail.size(), preprocessor.upwardsGraph.tail.size()) << "Vectors routingKitCch.up_tail and preprocessor.upwardsGraph.tail are of unequal length";
    for (int i = 0; i < routingKitCch.up_tail.size(); ++i) {
        EXPECT_EQ(routingKitCch.up_tail[i], preprocessor.upwardsGraph.tail[i]) << "Vectors routingKitCch.up_tail and preprocessor.upwardsGraph.tail differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.down_first_out.size(), preprocessor.downwardsGraph.adjacencyIndices.size()) << "Vectors routingKitCch.down_first_out and preprocessor.downwardsAdjacencyList are of unequal length";
    for (int i = 0; i < routingKitCch.down_first_out.size(); ++i) {
        EXPECT_EQ(routingKitCch.down_first_out[i], preprocessor.downwardsGraph.adjacencyIndices[i]) << "Vectors routingKitCch.down_first_out and preprocessor.downwardsAdjacencyList differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.down_head.size(), preprocessor.downwardsGraph.head.size()) << "Vectors routingKitCch.down_head and preprocessor.downwardsGraph.head are of unequal length";
    for (int i = 0; i < routingKitCch.down_head.size(); ++i) {
        EXPECT_EQ(routingKitCch.down_head[i], preprocessor.downwardsGraph.head[i]) << "Vectors routingKitCch.down_head and preprocessor.downwardsGraph.head differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.down_to_up.size(), preprocessor.downwardsToUpwardsGraph.size()) << "Vectors routingKitCch.down_to_up and preprocessor.downwardsToUpwardsGraph are of unequal length";
    for (int i = 0; i < routingKitCch.down_to_up.size(); ++i) {
        EXPECT_EQ(routingKitCch.down_to_up[i], preprocessor.downwardsToUpwardsGraph[i]) << "Vectors routingKitCch.down_to_up and preprocessor.downwardsToUpwardsGraph differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.input_arc_to_cch_arc.size(), preprocessor.inputEdgeToCchEdge.size()) << "Vectors routingKitCch.input_arc_to_cch_arc and preprocessor.inputArcToCchArc are of unequal length";
    for (int i = 0; i < routingKitCch.input_arc_to_cch_arc.size(); ++i) {
        EXPECT_EQ(routingKitCch.input_arc_to_cch_arc[i], preprocessor.inputEdgeToCchEdge[i]) << "Vectors routingKitCch.input_arc_to_cch_arc and preprocessor.inputArcToCchArc differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.forward_input_arc_of_cch.size(), preprocessor.forwardInputEdgeOfCchEdge.size()) << "Vectors routingKitCch.forward_input_arc_of_cch and preprocessor.forwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.forward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.forward_input_arc_of_cch[i], preprocessor.forwardInputEdgeOfCchEdge[i]) << "Vectors routingKitCch.forward_input_arc_of_cch and preprocessor.forwardInputArcOfCch differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.backward_input_arc_of_cch.size(), preprocessor.backwardInputEdgeOfCchEdge.size()) << "Vectors routingKitCch.backward_input_arc_of_cch and preprocessor.backwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.backward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.backward_input_arc_of_cch[i], preprocessor.backwardInputEdgeOfCchEdge[i]) << "Vectors routingKitCch.backward_input_arc_of_cch and preprocessor.backwardInputArcOfCch differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.extra_forward_input_arc_of_cch.size(), preprocessor.extraForwardInputEdgeOfCch.size()) << "Vectors routingKitCch.extra_forward_input_arc_of_cch and preprocessor.extraForwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.extra_forward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.extra_forward_input_arc_of_cch[i], preprocessor.extraForwardInputEdgeOfCch[i]) << "Vectors routingKitCch.extra_forward_input_arc_of_cch and preprocessor.extraForwardInputArcOfCch differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.extra_backward_input_arc_of_cch.size(), preprocessor.extraBackwardInputEdgeOfCch.size()) << "Vectors routingKitCch.extra_backward_input_arc_of_cch and preprocessor.extraBackwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.extra_backward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.extra_backward_input_arc_of_cch[i], preprocessor.extraBackwardInputEdgeOfCch[i]) << "Vectors routingKitCch.extra_backward_input_arc_of_cch and preprocessor.extraBackwardInputArcOfCch differ at index " << i;
    }
}

TEST(CchPreprocessorTest, Preprocess_WithMockGraph_ExpectSamePreprocessingAsRoutingKit) {
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes = {1.0, 11.0, 21.0, 31.0, 41.0};
    std::vector<float> longitudes = {50.0, 40.0, 30.0, 20.0, 10.0};
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);

    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(5, graph.tail, graph.head,
                                                                                      latitudes, longitudes);
    graph.vertexCount = latitudes.size();
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);

    // Act
    OptimizedKit::CchPreprocessor preprocessor(order, graph);

    // Assert
    ASSERT_EQ(routingKitCch.order.size(), preprocessor.order.size()) << "Vectors routingKitCch.order and preprocessor.order are of unequal length";
    for (int i = 0; i < routingKitCch.order.size(); ++i) {
        EXPECT_EQ(routingKitCch.order[i], preprocessor.order[i]) << "Vectors routingKitCch.order and preprocessor.order differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.rank.size(), preprocessor.rank.size()) << "Vectors routingKitCch.rank and preprocessor.rank are of unequal length";
    for (int i = 0; i < routingKitCch.rank.size(); ++i) {
        EXPECT_EQ(routingKitCch.rank[i], preprocessor.rank[i]) << "Vectors routingKitCch.rank and preprocessor.rank differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.up_first_out.size(), preprocessor.upwardsGraph.adjacencyIndices.size()) << "Vectors routingKitCch.up_first_out and preprocessor.upwardsAdjacencyList are of unequal length";
    for (int i = 0; i < routingKitCch.up_first_out.size(); ++i) {
        EXPECT_EQ(routingKitCch.up_first_out[i], preprocessor.upwardsGraph.adjacencyIndices[i]) << "Vectors routingKitCch.up_first_out and preprocessor.upwardsAdjacencyList differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.up_head.size(), preprocessor.upwardsGraph.head.size()) << "Vectors routingKitCch.up_head and preprocessor.upwardsGraph.head are of unequal length";
    for (int i = 0; i < routingKitCch.up_head.size(); ++i) {
        EXPECT_EQ(routingKitCch.up_head[i], preprocessor.upwardsGraph.head[i]) << "Vectors routingKitCch.up_head and preprocessor.upwardsGraph.head differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.up_tail.size(), preprocessor.upwardsGraph.tail.size()) << "Vectors routingKitCch.up_tail and preprocessor.upwardsGraph.tail are of unequal length";
    for (int i = 0; i < routingKitCch.up_tail.size(); ++i) {
        EXPECT_EQ(routingKitCch.up_tail[i], preprocessor.upwardsGraph.tail[i]) << "Vectors routingKitCch.up_tail and preprocessor.upwardsGraph.tail differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.down_first_out.size(), preprocessor.downwardsGraph.adjacencyIndices.size()) << "Vectors routingKitCch.down_first_out and preprocessor.downwardsAdjacencyList are of unequal length";
    for (int i = 0; i < routingKitCch.down_first_out.size(); ++i) {
        EXPECT_EQ(routingKitCch.down_first_out[i], preprocessor.downwardsGraph.adjacencyIndices[i]) << "Vectors routingKitCch.down_first_out and preprocessor.downwardsAdjacencyList differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.down_head.size(), preprocessor.downwardsGraph.head.size()) << "Vectors routingKitCch.down_head and preprocessor.downwardsGraph.head are of unequal length";
    for (int i = 0; i < routingKitCch.down_head.size(); ++i) {
        EXPECT_EQ(routingKitCch.down_head[i], preprocessor.downwardsGraph.head[i]) << "Vectors routingKitCch.down_head and preprocessor.downwardsGraph.head differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.down_to_up.size(), preprocessor.downwardsToUpwardsGraph.size()) << "Vectors routingKitCch.down_to_up and preprocessor.downwardsToUpwardsGraph are of unequal length";
    for (int i = 0; i < routingKitCch.down_to_up.size(); ++i) {
        EXPECT_EQ(routingKitCch.down_to_up[i], preprocessor.downwardsToUpwardsGraph[i]) << "Vectors routingKitCch.down_to_up and preprocessor.downwardsToUpwardsGraph differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.input_arc_to_cch_arc.size(), preprocessor.inputEdgeToCchEdge.size()) << "Vectors routingKitCch.input_arc_to_cch_arc and preprocessor.inputArcToCchArc are of unequal length";
    for (int i = 0; i < routingKitCch.input_arc_to_cch_arc.size(); ++i) {
        EXPECT_EQ(routingKitCch.input_arc_to_cch_arc[i], preprocessor.inputEdgeToCchEdge[i]) << "Vectors routingKitCch.input_arc_to_cch_arc and preprocessor.inputArcToCchArc differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.forward_input_arc_of_cch.size(), preprocessor.forwardInputEdgeOfCchEdge.size()) << "Vectors routingKitCch.forward_input_arc_of_cch and preprocessor.forwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.forward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.forward_input_arc_of_cch[i], preprocessor.forwardInputEdgeOfCchEdge[i]) << "Vectors routingKitCch.forward_input_arc_of_cch and preprocessor.forwardInputArcOfCch differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.backward_input_arc_of_cch.size(), preprocessor.backwardInputEdgeOfCchEdge.size()) << "Vectors routingKitCch.backward_input_arc_of_cch and preprocessor.backwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.backward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.backward_input_arc_of_cch[i], preprocessor.backwardInputEdgeOfCchEdge[i]) << "Vectors routingKitCch.backward_input_arc_of_cch and preprocessor.backwardInputArcOfCch differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.extra_forward_input_arc_of_cch.size(), preprocessor.extraForwardInputEdgeOfCch.size()) << "Vectors routingKitCch.extra_forward_input_arc_of_cch and preprocessor.extraForwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.extra_forward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.extra_forward_input_arc_of_cch[i], preprocessor.extraForwardInputEdgeOfCch[i]) << "Vectors routingKitCch.extra_forward_input_arc_of_cch and preprocessor.extraForwardInputArcOfCch differ at index " << i;
    }
    ASSERT_EQ(routingKitCch.extra_backward_input_arc_of_cch.size(), preprocessor.extraBackwardInputEdgeOfCch.size()) << "Vectors routingKitCch.extra_backward_input_arc_of_cch and preprocessor.extraBackwardInputArcOfCch are of unequal length";
    for (int i = 0; i < routingKitCch.extra_backward_input_arc_of_cch.size(); ++i) {
        EXPECT_EQ(routingKitCch.extra_backward_input_arc_of_cch[i], preprocessor.extraBackwardInputEdgeOfCch[i]) << "Vectors routingKitCch.extra_backward_input_arc_of_cch and preprocessor.extraBackwardInputArcOfCch differ at index " << i;
    }
}