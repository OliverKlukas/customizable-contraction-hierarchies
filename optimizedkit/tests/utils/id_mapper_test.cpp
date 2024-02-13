#include "gtest/gtest.h"
#include "utils/id_mapper.hpp"

using namespace OptimizedKit;

TEST(IdMapperTests, Constructor_WithFilter_SetsMapping) {
    // Arrange
    Filter filter = {true, false, true, true};

    // Act
    IdMapper idMapper(filter);

    // Assert
    ASSERT_EQ(idMapper.toLocal(0), 0);
    ASSERT_EQ(idMapper.toLocal(1), -1);
    ASSERT_EQ(idMapper.toLocal(2), 1);
    ASSERT_EQ(idMapper.toLocal(3), 2);
}

TEST(IdMapperTests, ToLocal_ValidGlobalId_ReturnsLocalId) {
    // Arrange
    Filter filter = {true, true, false, true};
    IdMapper idMapper(filter);
    auto expected = 2;

    // Act
    int actual = idMapper.toLocal(3);

    // Assert
    ASSERT_EQ(actual, expected);
}

TEST(IdMapperTests, LocalIdCount_GivenFilter_ReturnsCorrectCount) {
    // Arrange
    Filter filter = {true, false, true, false, true};
    IdMapper idMapper(filter);
    auto expected = 3;

    // Act
    unsigned actual = idMapper.getLocalIdCount();

    // Assert
    ASSERT_EQ(actual, expected);
}
