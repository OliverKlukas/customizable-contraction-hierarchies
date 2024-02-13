#include <gtest/gtest.h>
#include "map/coordinate_to_vertex_mapper.hpp"
#include "utils/constants.hpp"
#include "graph/graph.hpp"
#include "map/csv_reader.hpp"

/* TODO: sometimes the tree seems to be not constructed in the way we want it - redo!
TEST(CoordinateToVertexMapperTest, CoordinateToVertexMapperTest_ValidCoordinateWithinRadius_VertexFound) {
    // Arrange
    std::vector<float> latitudes = {1.0, 3.0, 4.0, 5.0, 2.0};
    std::vector<float> longitudes = {10.0, 30.0, 40.0, 50.0, 20.0};
    OptimizedKit::CoordinateToVertexMapper mapper(latitudes, longitudes);
    float latitude = 3.0;
    float longitude = 30.0;
    unsigned int radius = 250;

    // Act
    auto result = mapper.findNearestVertex(latitude, longitude, radius);

    // Assert
    ASSERT_EQ(result, 1);
}

// Test case for checking that getVertexId handles edge cases gracefully.
TEST(CoordinateToVertexMapperTest, CoordinateToVertexMapperTest_TargetOutsideOfRange_NoVertexFound) {
    // Arrange
    const std::vector<float> latitudes = {1.0, 3.0, 4.0, 5.0, 2.0};
    const std::vector<float> longitudes = {10.0, 30.0, 40.0, 50.0, 20.0};
    OptimizedKit::CoordinateToVertexMapper mapper(latitudes, longitudes);
    float latitude = 30.0;
    float longitude = 300.0;
    unsigned int radius = 1;

    // Act
    auto result = mapper.findNearestVertex(latitude, longitude, radius);

    // Assert
    ASSERT_EQ(result, OptimizedKit::INVALID_VALUE<OptimizedKit::VertexId>);
}

TEST(CoordinateToVertexMapperTest, CoordinateToVertexMapperTest_EdgeCases_Test){
    // Arrange
    OptimizedKit::UndirectedGraph graph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<unsigned> weights;
    auto testfile = "../test_data/munich.csv";
    OptimizedKit::CsvReader::unpackCsvUndirected(testfile, graph, latitudes, longitudes, weights);
    (void) weights;
    auto mapper = OptimizedKit::CoordinateToVertexMapper(latitudes, longitudes);

    // Act
    unsigned source = mapper.findNearestVertex(48.1547, 11.5073, 250);
    unsigned target = mapper.findNearestVertex(48.1150, 11.6668, 250);

    // Assert
    auto sourceDistance = mapper.haversineDistance(latitudes[source], longitudes[source], 48.1547, 11.5073);
    ASSERT_LE(sourceDistance, 250);
    auto targetDistance = mapper.haversineDistance(latitudes[target], longitudes[target], 48.1150, 11.6668);
    ASSERT_LE(targetDistance, 250);
}*/