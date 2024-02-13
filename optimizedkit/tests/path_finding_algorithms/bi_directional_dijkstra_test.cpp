#include <gtest/gtest.h>
#include <path_finding_algorithms/bi_directional_dijkstra.hpp>

class BiDirectionalDijkstraTest : public ::testing::Test {
protected:
    OptimizedKit::Graph upwardsGraph;
    OptimizedKit::Graph downwardsGraph;
    std::vector<unsigned> forwardWeights;
    std::vector<unsigned> backwardWeights;
    unsigned long vertexCount;

    void SetUp() override {
        upwardsGraph.tail = std::vector<OptimizedKit::VertexId>({0, 0, 1, 1, 2, 3, 3, 4, 4, 5});
        upwardsGraph.head = std::vector<OptimizedKit::VertexId>({1, 5, 5, 6, 4, 4, 5, 5, 6, 6});
        upwardsGraph.adjacencyIndices = {0, 2, 4, 5, 7, 9, 10, 10};
        forwardWeights = {OptimizedKit::INFINITY_WEIGHT<unsigned>, 3, 5, 10, OptimizedKit::INFINITY_WEIGHT<unsigned>,
                          OptimizedKit::INFINITY_WEIGHT<unsigned>, 0,
                          OptimizedKit::INFINITY_WEIGHT<unsigned>, OptimizedKit::INFINITY_WEIGHT<unsigned>,
                          OptimizedKit::INFINITY_WEIGHT<unsigned>};
        backwardWeights = {2,
                           OptimizedKit::INFINITY_WEIGHT<unsigned>,
                           OptimizedKit::INFINITY_WEIGHT<unsigned>,
                           OptimizedKit::INFINITY_WEIGHT<unsigned>,
                           7, 8, 2, 1, 0, 5};
        vertexCount = 10l;
    }
};

TEST_F(BiDirectionalDijkstraTest, Run_DisconnectedGraph_ReturnsNoPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = OptimizedKit::INFINITY_WEIGHT<unsigned>;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph);
    biDirectionalDijkstra.run(0, 6, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}

TEST_F(BiDirectionalDijkstraTest, Run_ValidOneFiveSingleResultQuery_ReturnsShortestDistanceAndPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = 5;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph);
    biDirectionalDijkstra.run(1, 5, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}

TEST_F(BiDirectionalDijkstraTest, Run_ValidOneTwoSingleResultQuery_ReturnsShortestDistanceAndPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = 13;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph);
    biDirectionalDijkstra.run(1, 2, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}

TEST_F(BiDirectionalDijkstraTest, Run_ValidThreeTwoSingleResultQuery_ReturnsShortestDistanceAndPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = 8;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph);
    biDirectionalDijkstra.run(3, 2, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}

TEST_F(BiDirectionalDijkstraTest, RunPairing_DisconnectedGraph_ReturnsNoPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = OptimizedKit::INFINITY_WEIGHT<unsigned>;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph, OptimizedKit::HeapType::PAIRING);
    biDirectionalDijkstra.run(0, 6, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}

TEST_F(BiDirectionalDijkstraTest, RunPairing_ValidOneFiveSingleResultQuery_ReturnsShortestDistanceAndPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = 5;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph, OptimizedKit::HeapType::PAIRING);
    biDirectionalDijkstra.run(1, 5, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}

TEST_F(BiDirectionalDijkstraTest, RunPairing_ValidOneTwoSingleResultQuery_ReturnsShortestDistanceAndPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = 13;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph, OptimizedKit::HeapType::PAIRING);
    biDirectionalDijkstra.run(1, 2, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}

TEST_F(BiDirectionalDijkstraTest, RunPairing_ValidThreeTwoSingleResultQuery_ReturnsShortestDistanceAndPath) {
    // Arrange
    OptimizedKit::CchGraph<unsigned> cchGraph(&upwardsGraph, &forwardWeights, &backwardWeights, vertexCount);
    auto expectedDistance = 8;

    // Act
    OptimizedKit::BiDirectionalDijkstra<unsigned> biDirectionalDijkstra(cchGraph, OptimizedKit::HeapType::PAIRING);
    biDirectionalDijkstra.run(3, 2, true);

    // Assert
    EXPECT_EQ(biDirectionalDijkstra.shortestPathLength, expectedDistance);
}
