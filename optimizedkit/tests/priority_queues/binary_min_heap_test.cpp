#include <gtest/gtest.h>
#include <random>
#include <priority_queues/binary_min_heap.hpp>

class BinaryMinHeapTest : public ::testing::Test {
protected:
    int NUM_VERTICES = 10;
    OptimizedKit::BinaryMinHeap<unsigned, unsigned> *heap;
    std::vector<unsigned> distances;

    void SetUp() override {
        distances.reserve(NUM_VERTICES);
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<unsigned> distribution(0, NUM_VERTICES);
        for (unsigned i = 0; i < NUM_VERTICES; i++)
            distances.push_back(distribution(generator) * distribution(generator));
    }

    void TearDown() override {
        delete heap;
    }
};

TEST_F(BinaryMinHeapTest, Insert_HeapNotEmpty_NodesExistInHeap) {
    // Arrange
    heap = new OptimizedKit::BinaryMinHeap<unsigned, unsigned>();

    // Act
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(distances[i], i);

    // Assert
    ASSERT_EQ(heap->size(), NUM_VERTICES);
}

TEST_F(BinaryMinHeapTest, DeleteMin_HeapNotEmpty_DeleteMinReturnsNodesInAscendingOrder) {
    // Arrange
    heap = new OptimizedKit::BinaryMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(i, i);

    // Act & Assert
    for (unsigned i = 0; i < NUM_VERTICES; i++) {
        auto actualId = heap->deleteMin();
        ASSERT_EQ(actualId, i);
    }
    ASSERT_EQ(heap->size(), 0);
}

TEST_F(BinaryMinHeapTest, DecreaseKey_NodeExistsAndNewKeyIsSmaller_KeyAndPositionAreUpdated) {
    // Arrange
    heap = new OptimizedKit::BinaryMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(10 + i, i);

    // Act
    auto notMin = heap->end() - 1;
    auto notMinId = notMin->id;
    heap->decreaseKey(notMinId, 0);

    // Assert
    ASSERT_EQ(heap->begin()->key, 0);
    ASSERT_EQ(heap->begin()->id, notMinId);
}

TEST_F(BinaryMinHeapTest, Clear_HeapNotEmpty_HeapBecomesEmpty) {
    // Arrange
    heap = new OptimizedKit::BinaryMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(distances[i], i);

    // Act
    heap->clear();

    // Assert
    ASSERT_TRUE(heap->isEmpty());
}

TEST_F(BinaryMinHeapTest, EmptyHeapOperations_HeapIsEmpty_ExceptionsThrown) {
    // Arrange
    heap = new OptimizedKit::BinaryMinHeap<unsigned, unsigned>();
    unsigned key = 5, id = 10;

    // Assert
    ASSERT_TRUE(heap->isEmpty());
    ASSERT_THROW(heap->deleteMin(), std::out_of_range);
    ASSERT_THROW(heap->decreaseKey(id, key), std::out_of_range);
}

TEST_F(BinaryMinHeapTest, InvalidDecreaseKey_NodeDoesNotExistOrNewKeyIsNotSmaller_ExceptionsThrown) {
    // Arrange
    heap = new OptimizedKit::BinaryMinHeap<unsigned, unsigned>();
    heap->insertOrUpdate(10, 1);
    unsigned key = 5, id = 2;

    // Act & Assert
    ASSERT_THROW(heap->decreaseKey(id, key), std::out_of_range);
    key = 15;
    ASSERT_THROW(heap->decreaseKey(1, key), std::invalid_argument);
}
