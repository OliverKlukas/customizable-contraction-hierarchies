#include <gtest/gtest.h>
#include <graph/graph.hpp>

TEST(GraphTest, AddArc_EmptyGraph_AddsArcAndFillsHeadAndTail) {
    // Arrange
    OptimizedKit::Graph graph;

    // Act
    graph.addEdge(0, 1);

    // Assert
    EXPECT_EQ(graph.head.size(), 1);
    EXPECT_EQ(graph.tail.size(), 1);
    EXPECT_EQ(graph.tail[0], 0);
    EXPECT_EQ(graph.head[0], 1);
}