#include <gtest/gtest.h>
#include "graph/graph.hpp"
#include "map/csv_reader.hpp"
#include "routingkit/nested_dissection.h"
#include "routingkit/customizable_contraction_hierarchy.h"
#include "utils/vector_helper.hpp"
#include "routingkit/geo_position_to_node.h"
#include "customizable_contraction_hierarchy/cch_preprocessor.hpp"
#include "customizable_contraction_hierarchy/cch_customizer.hpp"
#include "customizable_contraction_hierarchy/cch_query.hpp"
#include "../test_utils/utils.hpp"

TEST(CchQueryTest, Query_SingleQueryWithVideoMockGraph_SameQueryResultAsRoutingKit){
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes = {10.0, 20.0, 0.0, 20.0, 0.0, 20.0, 0.0};
    std::vector<float> longitudes = {0.0, 10.0, 10.0, 20.0, 20.0, 30.0, 30.0};
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);
    graph.addEdge(3, 5);
    graph.addEdge(4, 5);
    graph.addEdge(4, 6);
    graph.vertexCount = latitudes.size();
    std::vector<unsigned> weights = {2, 10, 3, 5, 0, 1, 2, 8, 7};
    unsigned source = 0, target = 6;
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                      graph.tail, graph.head,
                                                                                      latitudes, longitudes);
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    routingKitMetric.customize();
    RoutingKit::CustomizableContractionHierarchyQuery routingKitQuery(routingKitMetric);
    routingKitQuery.add_source(source).add_target(target).run();
    auto routingKitNodePath = routingKitQuery.get_node_path();
    auto routingKitArcPath = routingKitQuery.get_arc_path();
    auto routingKitTripWeight = routingKitQuery.get_distance();

    // Act
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    OptimizedKit::CchQuery query(customizer);
    query.run(source, target, false);
    auto vertexPath = query.getVertexPath();
    auto arcPath = query.getEdgePath();
    auto tripWeight = query.getQueryWeight();

    // Assert
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
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }

    ASSERT_EQ(routingKitTripWeight, tripWeight);
    ASSERT_EQ(routingKitArcPath.size(), arcPath.size()) << "Vectors routingKitArcPath and arcPath are of unequal length";
    for (int i = 0; i < routingKitArcPath.size(); ++i) {
        EXPECT_EQ(routingKitArcPath[i], arcPath[i]) << "Vectors routingKitArcPath and arcPath differ at index " << i;
    }
    ASSERT_EQ(routingKitNodePath.size(), vertexPath.size()) << "Vectors routingKitNodePath and vertexPath are of unequal length";
    for (int i = 0; i < routingKitNodePath.size(); ++i) {
        EXPECT_EQ(routingKitNodePath[i], vertexPath[i]) << "Vectors routingKitNodePath and vertexPath differ at index " << i;
    }
}

TEST(CchQueryTest, Query_SingleQueryWithMockGraph_SameQueryResultAsRoutingKit){
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes = {30.0, 10.0, 20.0, 30.0, 10.0, 20.0};
    std::vector<float> longitudes = {10.0, 10.0, 20.0, 30.0, 30.0, 40.0};
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);
    graph.addEdge(3, 5);
    graph.addEdge(4, 3);
    graph.addEdge(4, 5);
    graph.vertexCount = latitudes.size();
    std::vector<unsigned> weights = {1, 3, 3, 1, 1, 3, 1, 4, 1, 1};

    unsigned source = 0, target = 5;
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                      graph.tail, graph.head,
                                                                                      latitudes, longitudes);

    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    routingKitMetric.customize();
    RoutingKit::CustomizableContractionHierarchyQuery routingKitQuery(routingKitMetric);
    routingKitQuery.add_source(source).add_target(target).run();
    auto routingKitNodePath = routingKitQuery.get_node_path();
    auto routingKitArcPath = routingKitQuery.get_arc_path();
    auto routingKitTripWeight = routingKitQuery.get_distance();

    // Act
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    OptimizedKit::CchQuery query(customizer);
    query.run(source, target, false);
    auto tripWeight = query.getQueryWeight();
    auto vertexPath = query.getVertexPath();
    auto arcPath = query.getEdgePath();

    // Assert
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
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }

    ASSERT_EQ(routingKitTripWeight, tripWeight);
    ASSERT_EQ(routingKitArcPath.size(), arcPath.size()) << "Vectors routingKitArcPath and arcPath are of unequal length";
    for (int i = 0; i < routingKitArcPath.size(); ++i) {
        EXPECT_EQ(routingKitArcPath[i], arcPath[i]) << "Vectors routingKitArcPath and arcPath differ at index " << i;
    }
    ASSERT_EQ(routingKitNodePath.size(), vertexPath.size()) << "Vectors routingKitNodePath and vertexPath are of unequal length";
    for (int i = 0; i < routingKitNodePath.size(); ++i) {
        EXPECT_EQ(routingKitNodePath[i], vertexPath[i]) << "Vectors routingKitNodePath and vertexPath differ at index " << i;
    }
}

bool contains(std::vector<unsigned int> v, unsigned int i) {
    return std::find(v.begin(), v.end(), i) != v.end();
}

TEST(CchQueryTest, CchQuery_ExtendedTimedMediumSingleQueryWithOsmMap_SameQueryResultAsRoutingKit)
{
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<unsigned> weights;
    auto testfile = "../test_data/munich.csv";
    auto startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                      graph.tail, graph.head,
                                                                                      latitudes, longitudes);
    graph.vertexCount = latitudes.size();
    auto endTime = std::chrono::high_resolution_clock::now();
    printDuration("map & order prepared", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch preprocessed", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    routingKitMetric.customize();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch customized", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::GeoPositionToNode map_geo_position(latitudes, longitudes);
    unsigned source = map_geo_position.find_nearest_neighbor_within_radius(48.1547, 11.5073, 250).id;
    unsigned target = map_geo_position.find_nearest_neighbor_within_radius(48.1150, 11.6668, 250).id;
    endTime = std::chrono::high_resolution_clock::now();
    if(source == RoutingKit::invalid_id || target == RoutingKit::invalid_id){
        std::cout << "No source or target found!" << std::endl;
        throw std::runtime_error("No source or target found!");
    }
    printDuration("source & target coordinates -> vertexId generated", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchyQuery routingKitQuery(routingKitMetric);
    routingKitQuery.add_source(source).add_target(target).run();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch queried", startTime, endTime);

    auto routingKitNodePath = routingKitQuery.get_node_path();
    auto routingKitArcPath = routingKitQuery.get_arc_path();
    auto routingKitTripWeight = routingKitQuery.get_distance();

    // Act
    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch preprocessed", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch customized", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CchQuery query(customizer);
    query.run(source, target, true);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch queried", startTime, endTime);

    auto arcPath = query.getEdgePath();
    auto vertexPath = query.getVertexPath();
    auto tripWeight = query.getQueryWeight();

    // Optional: Print the results & analysis
    std::cout << "Find a route from " << source << " to " << target << std::endl;
    std::cout << "RoutingKit: " << std::endl;
    std::cout << "RoutingKit shortest distance: " << routingKitTripWeight << " meters" << std::endl;
    std::cout << "OptimizedKit shortest distance: " << tripWeight << " meters" << std::endl;
    std::cout << std::endl;

    std::cout << "RoutingKit shortest vertex path: ";
    for (auto vertex: routingKitNodePath) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::cout << "OptimizedKit shortest vertex path: ";
    for (auto vertex: vertexPath) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "OptimizedKit shortest local vertex path: ";
    for (auto vertex: vertexPath) {
        auto it = std::find(preprocessor.rank.begin(), preprocessor.rank.end(), vertex);
        if(it != preprocessor.rank.end()){
            std::cout << std::distance(preprocessor.rank.begin(), it) << " ";
        } else{
            std::cout << "not found" << " ";
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "RoutingKit shortest edge path: " << std::endl;
    for (auto arc: routingKitArcPath) {
        std::cout << "edge: " << arc << " with weight: " << routingKitMetric.input_weight[arc] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "OptimizedKit shortest edge path: " << std::endl;
    for (auto arc: arcPath) {
        std::cout << "edge: " << arc << " with weight: " << customizer.inputWeights[arc] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "OptimizedKit shortest local edge path: " << std::endl;
    for (auto arc: arcPath) {
        std::cout << "edge: " << preprocessor.inputEdgeToCchEdge[arc] << " with weight: " << customizer.inputWeights[arc] << std::endl;
    }
    std::cout << std::endl;

    unsigned diff = tripWeight - routingKitTripWeight;
    std::cout << "Difference in edge path: " << diff << std::endl;
    std::cout << std::endl;

    std::cout << "Arcs that are the different ones: " << std::endl;
    if(arcPath.size() > routingKitArcPath.size()){
        unsigned arc = 0;
        while(arc != routingKitArcPath.size()){
            if(arcPath[arc] != routingKitArcPath[arc]){
                std::cout << "DIFFERENCE: " << arcPath[arc] << " != " << routingKitArcPath[arc] << std::endl;
            }
            arc++;
        }
        while(arc != arcPath.size()){
            std::cout << "ADDITIONAL ARC IN ARCPATH: " << arcPath[arc] << std::endl;
            arc++;
        }
    } else{
        unsigned arc = 0;
        while(arc != arcPath.size()){
            if(arcPath[arc] != routingKitArcPath[arc]){
                std::cout << "DIFFERENCE: " << arcPath[arc] << " != " << routingKitArcPath[arc] << std::endl;
            }
            arc++;
        }
        while(arc != routingKitArcPath.size()){
            std::cout << "ADDITIONAL ARC IN ROUTINGKITARCPATH: " << routingKitArcPath[arc] << std::endl;
            arc++;
        }
    }
    std::cout << std::endl;

    std::cout << "Vertices that are the different ones: " << std::endl;
    if(vertexPath.size() > routingKitNodePath.size()){
        unsigned vertex = 0;
        while(vertex != routingKitNodePath.size()){
            if(vertexPath[vertex] != routingKitNodePath[vertex]){
                std::cout << "DIFFERENCE: " << vertexPath[vertex] << " != " << routingKitNodePath[vertex] << std::endl;
            }
            vertex++;
        }
        while(vertex != vertexPath.size()){
            std::cout << "ADDITIONAL VERTEX IN VERTEXPATH: " << vertexPath[vertex] << std::endl;
            vertex++;
        }
    } else {
        unsigned vertex = 0;
        while(vertex != vertexPath.size()){
            if(vertexPath[vertex] != routingKitNodePath[vertex]){
                std::cout << "DIFFERENCE: " << vertexPath[vertex] << " != " << routingKitNodePath[vertex] << std::endl;
            }
            vertex++;
        }
        while(vertex != routingKitNodePath.size()){
            std::cout << "ADDITIONAL VERTEX IN ROUTINGKITNODEPATH: " << routingKitNodePath[vertex] << std::endl;
            vertex++;
        }
    }

    // Assert
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
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }

    ASSERT_EQ(routingKitTripWeight, tripWeight);
    ASSERT_EQ(routingKitArcPath.size(), arcPath.size()) << "Vectors routingKitArcPath and arcPath are of unequal length";
    for (int i = 0; i < routingKitArcPath.size(); ++i) {
        EXPECT_EQ(routingKitArcPath[i], arcPath[i]) << "Vectors routingKitArcPath and arcPath differ at index " << i;
    }
    ASSERT_EQ(routingKitNodePath.size(), vertexPath.size()) << "Vectors routingKitNodePath and vertexPath are of unequal length";
    for (int i = 0; i < routingKitNodePath.size(); ++i) {
        EXPECT_EQ(routingKitNodePath[i], vertexPath[i]) << "Vectors routingKitNodePath and vertexPath differ at index " << i;
    }
}

TEST(CchQueryTest, CchQuery_SimpleQueryWithOsmMap_SameQueryResultAsRoutingKit)
{
    // Arrange
    OptimizedKit::Graph routingKitGraph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<unsigned> routingKitWeights;
    auto testfile = "../test_data/munich.csv";
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, routingKitGraph, latitudes, longitudes, routingKitWeights);
    auto routingKitOrder = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                                routingKitGraph.tail, routingKitGraph.head,
                                                                                                latitudes, longitudes);
    routingKitGraph.vertexCount = latitudes.size();
    OptimizedKit::Graph optimizedKitGraph(routingKitGraph);
    std::vector<unsigned> optimizedKitWeights;
    std::copy(routingKitWeights.begin(), routingKitWeights.end(), std::back_inserter(optimizedKitWeights));
    std::vector<unsigned> optimizedKitOrder;
    std::copy(routingKitOrder.begin(), routingKitOrder.end(), std::back_inserter(optimizedKitOrder));

    RoutingKit::GeoPositionToNode map_geo_position(latitudes, longitudes);
    unsigned source = map_geo_position.find_nearest_neighbor_within_radius(48.1081, 11.6257, 250).id;
    unsigned target = map_geo_position.find_nearest_neighbor_within_radius(48.1189, 11.6296, 250).id;
    if(source == RoutingKit::invalid_id || target == RoutingKit::invalid_id){
        std::cout << "No source or target found!" << std::endl;
        throw std::runtime_error("No source or target found!");
    }

    RoutingKit::CustomizableContractionHierarchy routingKitCch(routingKitOrder, routingKitGraph.tail, routingKitGraph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, routingKitWeights);
    routingKitMetric.customize();
    RoutingKit::CustomizableContractionHierarchyQuery routingKitQuery(routingKitMetric);
    routingKitQuery.add_source(source).add_target(target).run();

    auto routingKitNodePath = routingKitQuery.get_node_path();
    auto routingKitArcPath = routingKitQuery.get_arc_path();
    auto routingKitTripWeight = routingKitQuery.get_distance();

    // Act
    OptimizedKit::CchPreprocessor preprocessor(optimizedKitOrder, optimizedKitGraph);
    OptimizedKit::CchCustomizer customizer(preprocessor, optimizedKitWeights);
    customizer.baseCustomization();
    OptimizedKit::CchQuery query(customizer);
    query.run(source, target, false);

    auto arcPath = query.getEdgePath();
    auto vertexPath = query.getVertexPath();
    auto tripWeight = query.getQueryWeight();

    // Assert
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
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }

    ASSERT_EQ(routingKitTripWeight, tripWeight);
    ASSERT_EQ(routingKitArcPath.size(), arcPath.size()) << "Vectors routingKitArcPath and arcPath are of unequal length";
    for (int i = 0; i < routingKitArcPath.size(); ++i) {
        EXPECT_EQ(routingKitArcPath[i], arcPath[i]) << "Vectors routingKitArcPath and arcPath differ at index " << i;
    }
    ASSERT_EQ(routingKitNodePath.size(), vertexPath.size()) << "Vectors routingKitNodePath and vertexPath are of unequal length";
    for (int i = 0; i < routingKitNodePath.size(); ++i) {
        EXPECT_EQ(routingKitNodePath[i], vertexPath[i]) << "Vectors routingKitNodePath and vertexPath differ at index " << i;
    }
}

TEST(CchQueryTest, CchQuery_HardQueryWithOsmMap_SameQueryResultAsRoutingKit)
{
    // Arrange
    OptimizedKit::Graph routingKitGraph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<unsigned> routingKitWeights;
    auto testfile = "../test_data/munich.csv";
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, routingKitGraph, latitudes, longitudes, routingKitWeights);
    auto routingKitOrder = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                                routingKitGraph.tail, routingKitGraph.head,
                                                                                                latitudes, longitudes);
    routingKitGraph.vertexCount = latitudes.size();
    OptimizedKit::Graph optimizedKitGraph(routingKitGraph);
    std::vector<unsigned> optimizedKitWeights;
    std::copy(routingKitWeights.begin(), routingKitWeights.end(), std::back_inserter(optimizedKitWeights));
    std::vector<unsigned> optimizedKitOrder;
    std::copy(routingKitOrder.begin(), routingKitOrder.end(), std::back_inserter(optimizedKitOrder));

    RoutingKit::GeoPositionToNode map_geo_position(latitudes, longitudes);
    // unsigned source = map_geo_position.find_nearest_neighbor_within_radius(48.0772, 11.5102, 250).id;
    // unsigned target = map_geo_position.find_nearest_neighbor_within_radius(48.2123, 11.5549, 250).id;
    // unsigned source = map_geo_position.find_nearest_neighbor_within_radius(48.1138, 11.7183, 1000).id;
    // unsigned target = map_geo_position.find_nearest_neighbor_within_radius(48.1439, 11.5843, 1000).id;
    unsigned source = 40181;
    unsigned target = 17314;
    if(source == RoutingKit::invalid_id || target == RoutingKit::invalid_id){
        std::cout << "No source or target found!" << std::endl;
        throw std::runtime_error("No source or target found!");
    }

    RoutingKit::CustomizableContractionHierarchy routingKitCch(routingKitOrder, routingKitGraph.tail, routingKitGraph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, routingKitWeights);
    routingKitMetric.customize();
    RoutingKit::CustomizableContractionHierarchyQuery routingKitQuery(routingKitMetric);
    routingKitQuery.add_source(source).add_target(target).run();

    auto routingKitNodePath = routingKitQuery.get_node_path();
    auto routingKitArcPath = routingKitQuery.get_arc_path();
    auto routingKitTripWeight = routingKitQuery.get_distance();

    // Act
    OptimizedKit::CchPreprocessor preprocessor(optimizedKitOrder, optimizedKitGraph);
    OptimizedKit::CchCustomizer customizer(preprocessor, optimizedKitWeights);
    customizer.baseCustomization();
    OptimizedKit::CchQuery query(customizer);
    query.run(source, target, false);

    auto arcPath = query.getEdgePath();
    auto vertexPath = query.getVertexPath();
    auto tripWeight = query.getQueryWeight();

    // Assert
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
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }

    ASSERT_EQ(routingKitTripWeight, tripWeight);
    ASSERT_EQ(routingKitArcPath.size(), arcPath.size()) << "Vectors routingKitArcPath and arcPath are of unequal length";
    for (int i = 0; i < routingKitArcPath.size(); ++i) {
        EXPECT_EQ(routingKitArcPath[i], arcPath[i]) << "Vectors routingKitArcPath and arcPath differ at index " << i;
    }
    ASSERT_EQ(routingKitNodePath.size(), vertexPath.size()) << "Vectors routingKitNodePath and vertexPath are of unequal length";
    for (int i = 0; i < routingKitNodePath.size(); ++i) {
        EXPECT_EQ(routingKitNodePath[i], vertexPath[i]) << "Vectors routingKitNodePath and vertexPath differ at index " << i;
    }
}
