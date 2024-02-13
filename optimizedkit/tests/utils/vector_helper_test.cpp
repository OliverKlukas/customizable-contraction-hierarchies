#include "gtest/gtest.h"
#include "utils/vector_helper.hpp"

using namespace OptimizedKit;

TEST(VectorHelperTests, IsVectorSorted_SortedVector_ReturnsTrue) {
    // Arrange
    std::vector<unsigned> vec = {1, 2, 3, 4, 5};
    auto expected = true;

    // Act
    auto actual = isVectorSorted(vec);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(VectorHelperTests, IsVectorSorted_UnsortedVector_ReturnsFalse) {
    // Arrange
    std::vector<unsigned> vec = {1, 3, 2, 4, 5};
    auto expected = false;

    // Act
    auto actual = isVectorSorted(vec);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(VectorHelperTests, MaxElementOfVector_NonEmptyVector_ReturnsMax) {
    // Arrange
    std::vector<unsigned> vec = {1, 3, 5, 2, 4};
    auto expected = 5;

    // Act
    unsigned actual = maxElementOfVector(vec);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(VectorHelperTests, InvertedVectorMirror_ValidSortedVector_CorrectInverse) {
    // Arrange
    std::vector<unsigned> vec = {0, 2, 4};
    std::vector<unsigned> expected = {0, 1, 1, 2, 2, 3};

    // Act
    auto actual = constructAdjacencyIndices(vec, 5);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(VectorHelperTests, ConvertVector_ValidVectors_CorrectConvertedVector) {
    // Arrange
    std::vector<unsigned> vec = {0, 2, 4};
    std::vector<float> expected = {0.0, 2.0, 4.0};

    // Act
    auto actual = convertVector<unsigned, float>(vec);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(VectorHelperTests, FilterVectorInplace_ValidVectors_CorrectFilteredVector) {
    // Arrange
    std::vector<unsigned> actual = {0, 2, 4};
    Filter filter = {true, false, false};
    std::vector<unsigned> expected = {2,4};

    // Act
    removeElementsByFilterInplace(actual, filter);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(VectorHelperTests, RemoveIdsAndAdjustElementsInplace_ValidVectors_CorrectFilteredVector) {
    // Arrange
    std::vector<unsigned> actual = {3, 1, 6, 2, 0};
    Filter filter = {true, false, false, true, false, true, false}; // remove 0, 3, 5 and decrease 1 to 0, 6 to 3 and 2 to 1
    std::vector<unsigned> expected = {0, 3, 1};

    // Act
    adjustElementsToRemoveFilterInPlace(actual, filter);

    // Assert
    ASSERT_EQ(actual, expected);
}

