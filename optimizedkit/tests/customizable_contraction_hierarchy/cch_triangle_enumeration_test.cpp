#include <gtest/gtest.h>
#include "map/csv_reader.hpp"
#include "routingkit/nested_dissection.h"
#include "../test_utils/utils.hpp"
#include "customizable_contraction_hierarchy/cch_preprocessor.hpp"
#include "customizable_contraction_hierarchy/cch_customizer.hpp"

TEST(CchTriangleEnumerationTest, Preprocess_WithOsmMap_VerifyAllTrianglesCorrect) {
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
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    endTime = std::chrono::high_resolution_clock::now();
    printDuration("optimizedkit cch customized", startTime, endTime);

    // Act
    auto triangleVerification = [&](OptimizedKit::EdgeId lowerArc, OptimizedKit::EdgeId intermediateArc,
                                    OptimizedKit::EdgeId upperArc, OptimizedKit::VertexId lowerVertex,
                                    OptimizedKit::VertexId intermediateVertex, OptimizedKit::VertexId upperVertex) {
        // Assert lower vertex is smallest overall and exists in upwards graph.
        assert(lowerVertex < intermediateVertex);
        assert(lowerArc < intermediateArc);
        assert(preprocessor.upwardsGraph.tail[lowerArc] == lowerVertex);
        assert(preprocessor.upwardsGraph.head[lowerArc] == intermediateVertex);

        // Assert intermediate vertex is in-between upper and lower and exists in upwards graph.
        assert(intermediateVertex < upperVertex);
        assert(intermediateArc < upperArc);
        assert(preprocessor.upwardsGraph.tail[intermediateArc] == lowerVertex);
        assert(preprocessor.upwardsGraph.head[intermediateArc] == upperVertex);

        // Assert upper vertex is largest overall and exists in upwards graph.
        assert(preprocessor.upwardsGraph.tail[upperArc] == intermediateVertex);
        assert(preprocessor.upwardsGraph.head[upperArc] == upperVertex);
        return true;
    };

    // Assert
    for (OptimizedKit::EdgeId arc = 0; arc < preprocessor.cchEdgeCount(); ++arc)
        enumerateUpperTriangles(preprocessor, arc, triangleVerification);

    for (OptimizedKit::EdgeId arc = 0; arc < preprocessor.cchEdgeCount(); ++arc)
        enumerateIntermediateTriangles(preprocessor, arc, triangleVerification);

    for (OptimizedKit::EdgeId arc = 0; arc < preprocessor.cchEdgeCount(); ++arc)
        enumerateLowerTriangles(preprocessor, arc, triangleVerification);
}
