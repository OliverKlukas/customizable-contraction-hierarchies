#include "gtest/gtest.h"
#include "routingkit/graph_util.h"
#include "utils/math.hpp"

using namespace OptimizedKit;

TEST(MathTests, AdditionOverflow_UnsignedNoOverflow_ReturnsFalse) {
    // Arrange
    auto *result = new unsigned int[(sizeof(unsigned int))];
    unsigned int a = 100;
    unsigned int b = 200;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_FALSE(overflow);
    EXPECT_EQ(*result, 300);
}

TEST(MathTests, AdditionOverflow_UnsignedOverflow_ReturnsTrue) {
    // Arrange
    auto *result = new unsigned int[(sizeof(unsigned int))];
    unsigned int a = std::numeric_limits<unsigned int>::max();
    unsigned int b = 100;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_TRUE(overflow);
    EXPECT_EQ(*result, std::numeric_limits<unsigned int>::max());
}

TEST(MathTests, AdditionOverflow_DoubleNoOverflow_ReturnsFalse) {
    // Arrange
    auto *result = new double[(sizeof(double))];
    double a = 1.5;
    double b = 2.5;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_FALSE(overflow);
    EXPECT_EQ(*result, 4.0);
}

TEST(MathTests, AdditionOverflow_DoubleOverflow_ReturnsTrue) {
    // Arrange
    auto *result = new double[(sizeof(double))];
    double a = std::numeric_limits<double>::max();
    double b = 1.0;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_TRUE(overflow);
    EXPECT_EQ(*result, std::numeric_limits<double>::max());
}

TEST(MathTests, AdditionOverflow_FloatNoOverflow_ReturnsFalse) {
    // Arrange
    auto *result = new float[(sizeof(float))];
    float a = 1.5f;
    float b = 2.5f;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_FALSE(overflow);
    EXPECT_EQ(*result, 4.0f);
}

TEST(MathTests, AdditionOverflow_FloatOverflow_ReturnsTrue) {
    // Arrange
    auto *result = new float[(sizeof(float))];
    float a = std::numeric_limits<float>::max();
    float b = 1.0f;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_TRUE(overflow);
    EXPECT_EQ(*result, std::numeric_limits<float>::max());
}

TEST(MathTests, AdditionOverflow_IntNoOverflow_ReturnsFalse) {
    // Arrange
    auto *result = new int[(sizeof(int))];
    int a = 100;
    int b = 200;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_FALSE(overflow);
    EXPECT_EQ(*result, 300);
}

TEST(MathTests, AdditionOverflow_IntOverflow_ReturnsTrue) {
    // Arrange
    auto *result = new int[(sizeof(int))];
    int a = std::numeric_limits<int>::max();
    int b = 100;

    // Act
    auto overflow = positiveOverflowAddition(result, a, b);

    // Assert
    EXPECT_TRUE(overflow);
    EXPECT_EQ(*result, std::numeric_limits<int>::max());
}
