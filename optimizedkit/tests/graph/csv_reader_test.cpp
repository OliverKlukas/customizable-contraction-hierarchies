#include <gtest/gtest.h>
#include "map/csv_reader.hpp"

TEST(CsvReaderTest, UnpackCsvUndirected_validFile_returnsUndirectedGraphFloat) {
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<float> weights;

    // Act
    auto testfile = "../test_data/munich.csv";
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);
    graph.vertexCount = latitudes.size();

    // Assert
    EXPECT_EQ(latitudes.size(), 63024);
    EXPECT_EQ(graph.getEdgeCount(), 107816);
}

TEST(CsvReaderTest, UnpackCsvUndirected_validFile_returnsUndirectedGraphUnsigned) {
    // Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<unsigned> weights;

    // Act
    auto testfile = "../test_data/munich.csv";
    OptimizedKit::CsvReader::extractGraphFromCsv(testfile, graph, latitudes, longitudes, weights);
    graph.vertexCount = latitudes.size();

    // Assert
    EXPECT_EQ(latitudes.size(), 63024);
    EXPECT_EQ(graph.getEdgeCount(), 107816);
}