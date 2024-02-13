#include <gtest/gtest.h>
#include "graph/graph.hpp"
#include "customizable_contraction_hierarchy/cch_preprocessor.hpp"
#include "customizable_contraction_hierarchy/cch_customizer.hpp"
#include "customizable_contraction_hierarchy/cch_query.hpp"
#include "customizable_contraction_hierarchy/customizable_contraction_hierarchy.hpp"
class CchWrapper : public ::testing::Test {
protected:
    OptimizedKit::Graph graph;
    std::vector<float> latitudes, longitudes;
    std::vector<unsigned> weights;
    std::vector<OptimizedKit::VertexId> order;
    OptimizedKit::VertexId source{}, target{};

    void SetUp() override {
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
        weights = {1, 3, 3, 1, 1, 3, 1, 4, 1, 1};
        source = 0;
        target = 5;
        order = {1, 0, 5, 3, 4, 2};
    }
};

TEST_F(CchWrapper, Query_GraphOrderWeights_SameResultsAsSeperateCompenents){
    // Arrange
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    OptimizedKit::CchQuery query(customizer);
    query.run(source, target);
    auto expectedWeight = query.getQueryWeight();
    auto expectedVertexPath = query.getVertexPath();
    auto expectedArcPath = query.getEdgePath();

    // Act
    OptimizedKit::CustomizableContractionHierarchy cch(order, graph, weights);
    cch.run(source, target);
    auto actualWeight = cch.getQueryWeight();
    auto actualVertexPath = cch.getVertexPath();
    auto actualArcPath = cch.getEdgePath();

    // Assert
    ASSERT_EQ(actualWeight, expectedWeight);
    ASSERT_EQ(actualArcPath.size(), expectedArcPath.size()) << "Vectors actualArcPath and expectedArcPath are of unequal length";
    for (int i = 0; i < actualArcPath.size(); ++i) {
        EXPECT_EQ(actualArcPath[i], expectedArcPath[i]) << "Vectors actualArcPath and expectedArcPath differ at index " << i;
    }
    ASSERT_EQ(actualVertexPath.size(), expectedVertexPath.size()) << "Vectors actualVertexPath and expectedVertexPath are of unequal length";
    for (int i = 0; i < actualVertexPath.size(); ++i) {
        EXPECT_EQ(actualVertexPath[i], expectedVertexPath[i]) << "Vectors actualVertexPath and expectedVertexPath differ at index " << i;
    }
}

TEST_F(CchWrapper, QueryResetQuery_GraphOrderWeights_SameResultsAsSeperateCompenents){
    // Arrange
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.baseCustomization();
    OptimizedKit::CchQuery query(customizer);
    query.run(source, target);
    const std::vector<unsigned> updatedWeights {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    customizer.reset(updatedWeights);
    customizer.baseCustomization();
    query.run(source, target);
    auto expectedWeight = query.getQueryWeight();
    auto expectedVertexPath = query.getVertexPath();
    auto expectedArcPath = query.getEdgePath();

    // Act
    OptimizedKit::CustomizableContractionHierarchy cch(order, graph, weights);
    cch.run(source, target);
    cch.reset(updatedWeights);
    cch.run(source, target);
    auto actualWeight = cch.getQueryWeight();
    auto actualVertexPath = cch.getVertexPath();
    auto actualArcPath = cch.getEdgePath();

    // Assert
    ASSERT_EQ(actualWeight, expectedWeight);
    ASSERT_EQ(actualArcPath.size(), expectedArcPath.size()) << "Vectors actualArcPath and expectedArcPath are of unequal length";
    for (int i = 0; i < actualArcPath.size(); ++i) {
        EXPECT_EQ(actualArcPath[i], expectedArcPath[i]) << "Vectors actualArcPath and expectedArcPath differ at index " << i;
    }
    ASSERT_EQ(actualVertexPath.size(), expectedVertexPath.size()) << "Vectors actualVertexPath and expectedVertexPath are of unequal length";
    for (int i = 0; i < actualVertexPath.size(); ++i) {
        EXPECT_EQ(actualVertexPath[i], expectedVertexPath[i]) << "Vectors actualVertexPath and expectedVertexPath differ at index " << i;
    }
}
