#include "gtest/gtest.h"
#include "utils/permutation.hpp"

using namespace OptimizedKit;

TEST(PermutationTests, IsPermutation_ValidPermutation_ReturnsTrue) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    auto expected = true;

    // Act
    auto actual = isPermutation(permutation);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, IsPermutation_InvalidPermutation_ReturnsFalse) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 6};
    auto expected = false;

    // Act
    auto actual = isPermutation(permutation);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, ApplyPermutation_ValidPermutation_ReturnsPermutedVector) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    std::vector<unsigned> vec = {1, 2, 3, 4};
    std::vector<unsigned> expected = {2, 3, 4, 1};

    // Act
    auto actual = applyPermutation(permutation, vec);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, ApplyPermutation_ValidPermutation_ReturnsPermutedVectorAndMovedVector) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    std::vector<unsigned> vec = {1, 2, 3, 4};
    std::vector<unsigned> expected = {2, 3, 4, 1};

    // Act
    auto actual = applyPermutation(permutation, std::move(vec));

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, InvertPermutation_ValidPermutation_ReturnsInversePermutation) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    std::vector<unsigned> expected = {3, 0, 1, 2};

    // Act
    auto actual = invertPermutation(permutation);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, InplaceApplyPermutationToElementsOf_ValidPermutation_ReturnsPermutedVector) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    std::vector<unsigned> vec = {1, 2, 3, 0};
    std::vector<unsigned> expected = {2, 3, 0, 1};

    // Act
    inplaceApplyPermutationToElementsOf(permutation, vec);

    // Assert
    ASSERT_EQ(vec, expected);
}

TEST(PermutationTests, ApplyPermutationToElementsOf_ValidPermutation_ReturnsPermutedVector) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    std::vector<unsigned> vec = {1, 2, 3, 0};
    std::vector<unsigned> expected = {2, 3, 0, 1};

    // Act
    auto actual = applyPermutationToElementsOf(permutation, vec);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, ApplyInversePermutation_ValidPermutation_ReturnsInversePermutedVector) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    std::vector<unsigned> vec = {1, 2, 3, 4};
    std::vector<unsigned> expected = {4, 1, 2, 3};

    // Act
    auto actual = applyInversePermutation(permutation, vec);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, ApplyInversePermutation_ValidPermutation_ReturnsInversePermutedVectorAndMovedVector) {
    // Arrange
    std::vector<unsigned> permutation = {1, 2, 3, 0};
    std::vector<unsigned> vec = {1, 2, 3, 4};
    std::vector<unsigned> expected = {4, 1, 2, 3};

    // Act
    auto actual = applyInversePermutation(permutation, std::move(vec));

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests, ChainPermutationFirstLeftThenRight_ValidPermutations_ReturnsChainedPermutation) {
    // Arrange
    std::vector<unsigned> permutationLeft = {1, 2, 3, 0};
    std::vector<unsigned> permutationRight = {1, 2, 3, 0};
    std::vector<unsigned> expected = {2, 3, 0, 1};

    // Act
    auto actual = chainPermutationFirstLeftThenRight(permutationLeft, permutationRight);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(PermutationTests,
     ComputeInverseSortPermutationFirstByTailThenByHeadAndApplySortToTail_validGraph_ReturnsInverseSortPermutationAndSortedTail) {
    // Arrange
    std::vector<unsigned> head = {3, 1, 0, 2};
    std::vector<unsigned> tail = {1, 2, 3, 0};
    std::vector<unsigned> expectedPermutation = { 1, 2, 3, 0 };
    std::vector<unsigned> expectedSortedTail = {0, 1, 2, 3};

    // Act
    auto actualPermutation = computeInverseSortPermutationFirstByTailThenByHeadAndApplySortToTail(tail, head);

    // Assert
    ASSERT_EQ(actualPermutation, expectedPermutation);
    ASSERT_EQ(tail, expectedSortedTail);
    ASSERT_TRUE(isPermutation(actualPermutation));
    ASSERT_TRUE(isVectorSorted(tail));
}

TEST(PermutationTests,
     ComputeSortPermutationFirstByTailThenByHeadAndApplySortToTail_validGraph_ReturnsSortPermutationAndSortedTail) {
    // Arrange
    std::vector<unsigned> head = {3, 1, 0, 2};
    std::vector<unsigned> tail = {1, 2, 3, 0};
    std::vector<unsigned> expectedPermutation = { 3, 0, 1, 2 };
    std::vector<unsigned> expectedSortedTail = {0, 1, 2, 3};

    // Act
    auto actualPermutation = computeSortPermutationFirstByTailThenByHeadAndApplySortToTail(tail, head);

    // Assert
    ASSERT_EQ(actualPermutation, expectedPermutation);
    ASSERT_EQ(tail, expectedSortedTail);
    ASSERT_TRUE(isPermutation(actualPermutation));
    ASSERT_TRUE(isVectorSorted(tail));
}

TEST(PermutationTests, ComputeStableSortPermutation_RandomHeadVector_ReturnsStableSortPermutation) {
    // Arrange
    std::vector<unsigned> head = {3, 1, 0, 2};
    std::vector<unsigned> expectedPermutation = { 2, 1, 3, 0 };

    // Act
    auto actualPermutation = computeStableSortPermutation(head);

    // Assert
    ASSERT_EQ(actualPermutation, expectedPermutation);
    ASSERT_TRUE(isPermutation(actualPermutation));
}

TEST(PermutationTests, ComputeInverseStableSortPermutation_RandomHeadVector_ReturnsInverseStableSortPermutation) {
    // Arrange
    std::vector<unsigned> head = {3, 1, 0, 2};
    std::vector<unsigned> expectedPermutation = {3, 1, 0, 2};

    // Act
    auto actualPermutation = computeInverseStableSortPermutation(head);

    // Assert
    ASSERT_EQ(actualPermutation, expectedPermutation);
}

TEST(PermutationTests, ComputeStableSortPermutation_LongerHeadVector_ReturnsStableSortPermutation) {
    // Arrange
    std::vector<unsigned> head = {0, 4, 4, 2, 3, 3};
    std::vector<unsigned> expectedPermutation = {0, 3, 4, 5, 1, 2};

    // Act
    auto actualPermutation = computeStableSortPermutation(head);

    // Assert
    ASSERT_EQ(actualPermutation, expectedPermutation);
}

