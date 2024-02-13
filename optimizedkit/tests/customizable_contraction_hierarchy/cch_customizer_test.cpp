#include <gtest/gtest.h>
#include <routingkit/customizable_contraction_hierarchy.h>
#include <routingkit/contraction_hierarchy.h>
#include <routingkit/nested_dissection.h>
#include <routingkit/osm_simple.h>
#include <routingkit/inverse_vector.h>
#include <random>
#include "graph/graph.hpp"
#include "customizable_contraction_hierarchy/cch_preprocessor.hpp"
#include "map/csv_reader.hpp"
#include "customizable_contraction_hierarchy/cch_customizer.hpp"
#include "../test_utils/utils.hpp"
#include "customizable_contraction_hierarchy/cch_query.hpp"

TEST(CchCustomizerTest, BaseCustomize_WithMockGraph_CustomizeAsRoutingKit) {
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

    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                      graph.tail, graph.head,
                                                                                      latitudes, longitudes);

    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    routingKitMetric.customize();

    // Act
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();

    // Assert
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size())
                                << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i])
                            << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size())
                                << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i])
                            << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }
}

TEST(CchCustomizerTest, BaseCustomize_WithOsmMap_CustomizeAsRoutingKit){
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<unsigned> weights;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    auto testfile = "../test_data/munich.csv";
    auto startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(), graph.tail,
                                                                                      graph.head, latitudes,
                                                                                      longitudes);
    graph.vertexCount = latitudes.size();
    auto endTime = std::chrono::high_resolution_clock::now();
    printDuration("map, weights & order prepared", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    routingKitMetric.customize();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch customized", startTime, endTime);

    // Act
    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch customized", startTime, endTime);

    // Assert
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }
}

TEST(CchCustomizerTest, PerfectCustomize_WithMockGraph_RemovesEdges) {
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

    std::vector<unsigned> weights = {1, 3, 3, 1, 1, 3, 1, 4, 1, 1};

    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                      graph.tail, graph.head,
                                                                                      latitudes, longitudes);
    graph.vertexCount = latitudes.size();
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    auto sizePreCustomization = preprocessor.cchEdgeCount();

    // Act
    customizer.perfectCustomization();
    auto sizePostCustomization = preprocessor.cchEdgeCount();

    // Assert
    ASSERT_LE(sizePostCustomization, sizePreCustomization);
    std::cout << "INFO: Size before perfect customization: " << sizePreCustomization << std::endl;
    std::cout << "INFO: Size after perfect customization: " << sizePostCustomization << std::endl;
    if(sizePostCustomization == sizePreCustomization){
        std::cout << "INFO: No edges removed" << std::endl;
    } else {
        std::cout << "INFO: Removed " << sizePreCustomization - sizePostCustomization << " edges from cch" << std::endl;
    }
}

TEST(CchCustomizerTest, PerfectCustomize_WithOsmMap_RemovesEdges){
    // Arrange
    /*OptimizedKit::Graph graph;
    std::vector<unsigned> weights;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    auto testfile = "../test_data/munich.csv";
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);*/
    auto routingKitGraph = RoutingKit::simple_load_osm_car_routing_graph_from_pbf("../test_data/munich.osm.pbf");
    OptimizedKit::Graph graph;
    graph.tail = RoutingKit::invert_inverse_vector(routingKitGraph.first_out);
    graph.head = routingKitGraph.head;
    graph.adjacencyIndices = routingKitGraph.first_out;
    std::vector<float> latitudes = routingKitGraph.latitude;
    std::vector<float> longitudes = routingKitGraph.longitude;
    std::vector<unsigned> weights = routingKitGraph.geo_distance;

    RoutingKit::ContractionHierarchy ch = RoutingKit::ContractionHierarchy::build(latitudes.size(), graph.tail,
                                                                                  graph.head, weights);
    //auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(), graph.tail, graph.head, latitudes, longitudes);
    graph.vertexCount = latitudes.size();
    OptimizedKit::CchPreprocessor preprocessor(ch.order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    // customizer.baseCustomization();
    auto sizePreCustomization = preprocessor.cchEdgeCount();

    // Act
    customizer.perfectCustomization();
    auto sizePostCustomization = preprocessor.cchEdgeCount();

    // Assert
    ASSERT_LE(sizePostCustomization, sizePreCustomization);
    std::cout << "INFO: Size before perfect customization: " << sizePreCustomization << std::endl;
    std::cout << "INFO: Size after perfect customization: " << sizePostCustomization << std::endl;
    if(sizePostCustomization == sizePreCustomization){
        std::cout << "INFO: No edges removed" << std::endl;
    } else {
        std::cout << "INFO: Removed " << sizePreCustomization - sizePostCustomization << " edges from cch" << std::endl;
    }
}

TEST(CchCustomizerTest, Update_WithMockGraphAndPartialUpdate_CustomizeAsRoutingKit){
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

    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                      graph.tail, graph.head,
                                                                                      latitudes, longitudes);

    // Act
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    routingKitMetric.customize();
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();

    std::vector<OptimizedKit::EdgeId> updateArcIds = {3, 6, 1};
    for(auto arc : updateArcIds){
        weights[arc] = 42;
    }

    RoutingKit::CustomizableContractionHierarchyPartialCustomization routingKitPartialCustomization(routingKitCch);
    routingKitPartialCustomization.reset();
    for(auto arc: updateArcIds){
        routingKitPartialCustomization.update_arc(arc);
    }
    routingKitPartialCustomization.customize(routingKitMetric);
    customizer.update(updateArcIds);

    // Assert
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }
}

TEST(CchCustomizerTest, Update_WithOsmGraphAndPartialUpdate_CustomizeAsRoutingKit){
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<unsigned> weights;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    auto testfile = "../test_data/munich.csv";
    auto startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(), graph.tail,
                                                                                      graph.head, latitudes,
                                                                                      longitudes);
    graph.vertexCount = latitudes.size();
    auto endTime = std::chrono::high_resolution_clock::now();
    printDuration("map, weights & order prepared", startTime, endTime);

    // Act
    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    routingKitMetric.customize();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch customized", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch customized", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    std::vector<OptimizedKit::EdgeId> updateArcIds;
    updateArcIds.reserve(300);
    std::mt19937 gen(42);
    std::uniform_int_distribution<OptimizedKit::EdgeId> dis(0, weights.size() - 1);
    for (size_t i = 0; i < 300; ++i) {
        updateArcIds.push_back(dis(gen));
        weights[updateArcIds[i]] = dis(gen);
    }
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("weights manipulated", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchyPartialCustomization routingKitPartialCustomization(routingKitCch);
    routingKitPartialCustomization.reset();
    for(auto arc: updateArcIds){
        routingKitPartialCustomization.update_arc(arc);
    }
    routingKitPartialCustomization.customize(routingKitMetric);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch updated", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    customizer.update(updateArcIds);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch updated", startTime, endTime);

    // Assert
    ASSERT_EQ(routingKitMetric.forward.size(), customizer.forwardWeights.size()) << "Vectors routingKitMetric.forward and customizer.forwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.forward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.forward[i], customizer.forwardWeights[i]) << "Vectors routingKitMetric.forward and customizer.forwardWeights differ at index " << i;
    }
    ASSERT_EQ(routingKitMetric.backward.size(), customizer.backwardWeights.size()) << "Vectors routingKitMetric.backward and customizer.backwardWeights are of unequal length";
    for (int i = 0; i < routingKitMetric.backward.size(); ++i) {
        EXPECT_EQ(routingKitMetric.backward[i], customizer.backwardWeights[i]) << "Vectors routingKitMetric.backward and customizer.backwardWeights differ at index " << i;
    }
}

TEST(CchCustomizerTest, Update_WithOsmGraphAndPartialUpdate_SameDistnacesAndPathsAsRoutingKit){
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<unsigned> weights;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    auto testfile = "../test_data/munich.csv";
    auto startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(), graph.tail,
                                                                                      graph.head, latitudes,
                                                                                      longitudes);
    graph.vertexCount = latitudes.size();
    auto endTime = std::chrono::high_resolution_clock::now();
    printDuration("map, weights & order prepared", startTime, endTime);

    // Act
    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchy routingKitCch(order, graph.tail, graph.head);
    RoutingKit::CustomizableContractionHierarchyMetric routingKitMetric(routingKitCch, weights);
    auto routingKitCH = routingKitMetric.build_contraction_hierarchy_using_perfect_witness_search();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch perfect customized", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.perfectCustomization();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch perfect customized", startTime, endTime);

    std::mt19937 gen(42);
    std::uniform_int_distribution<OptimizedKit::VertexId> query_dis(0, graph.vertexCount - 1);
    startTime = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 100; ++i) {
        OptimizedKit::VertexId source = query_dis(gen);
        OptimizedKit::VertexId target = query_dis(gen);
        OptimizedKit::CchQuery query(customizer);
        query.run(source, target);
        RoutingKit::ContractionHierarchyQuery routingKitQuery(routingKitCH);
        routingKitQuery.reset().add_source(source).add_target(target).run();
        ASSERT_EQ(query.getQueryWeight(), routingKitQuery.get_distance()) << "Distances differ for source " << source << " and target " << target;
        // ASSERT_EQ(optimizedKitQuery.getVertexPath(), routingKitQuery.get_node_path()) << "Paths differ for source " << source << " and target " << target;
    }
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("compared queries are equal", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    std::vector<OptimizedKit::EdgeId> updateArcIds;
    updateArcIds.reserve(300);
    std::uniform_int_distribution<OptimizedKit::EdgeId> updated_dis(0, weights.size() - 1);
    for (size_t i = 0; i < 300; ++i) {
        updateArcIds.push_back(updated_dis(gen));
        weights[updateArcIds[i]] = updated_dis(gen);
    }
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("weights manipulated", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    RoutingKit::CustomizableContractionHierarchyPartialCustomization routingKitPartialCustomization(routingKitCch);
    routingKitPartialCustomization.reset();
    for(auto arc: updateArcIds){
        routingKitPartialCustomization.update_arc(arc);
    }
    routingKitPartialCustomization.customize(routingKitMetric);
    routingKitCH = routingKitMetric.build_contraction_hierarchy_using_perfect_witness_search();
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("routingkit cch updated", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    customizer.update(updateArcIds);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch updated", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 100; ++i) {
        OptimizedKit::VertexId source = query_dis(gen);
        OptimizedKit::VertexId target = query_dis(gen);
        OptimizedKit::CchQuery query(customizer);
        query.run(source, target);
        RoutingKit::ContractionHierarchyQuery routingKitQuery(routingKitCH);
        routingKitQuery.reset().add_source(source).add_target(target).run();
        ASSERT_EQ(query.getQueryWeight(), routingKitQuery.get_distance()) << "Distances differ for source " << source << " and target " << target;
        // ASSERT_EQ(optimizedKitQuery.getVertexPath(), routingKitQuery.get_node_path()) << "Paths differ for source " << source << " and target " << target;
    }
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("compared queries are still equal", startTime, endTime);
}