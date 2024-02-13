#include <gtest/gtest.h>
#include <random>
#include <priority_queues/pairing_min_heap.hpp>

class PairingMinHeapTest : public ::testing::Test {
protected:
    int NUM_VERTICES = 10;
    OptimizedKit::PairingMinHeap<unsigned, unsigned> *heap;
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
        heap->clear();
        delete heap;
    }
};

TEST_F(PairingMinHeapTest, Insert_HeapNotEmpty_NodesExistInHeap) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();

    // Act
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(distances[i], i);

    // Assert
    ASSERT_EQ(heap->size(), NUM_VERTICES);
}

TEST_F(PairingMinHeapTest, DeleteMin_HeapNotEmpty_DeleteMinReturnsNodesInAscendingOrder) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(i, i);

    // Act & Assert
    for (unsigned i = 0; i < NUM_VERTICES; i++) {
        auto actualId = heap->deleteMin();
        ASSERT_EQ(actualId, i);
    }
    ASSERT_EQ(heap->size(), 0);
}

TEST_F(PairingMinHeapTest, DecreaseKey_NodeExistsAndNewKeyIsSmaller_KeyAndPositionAreUpdated) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(10 + i, i);

    // Act
    auto notMinId = NUM_VERTICES-1;
    heap->decreaseKey(notMinId, 0);

    // Assert
    auto minKey = heap->peek();
    ASSERT_EQ(minKey, 0);
    auto minId = heap->deleteMin();
    ASSERT_EQ(minId, notMinId);
}

TEST_F(PairingMinHeapTest, Clear_HeapNotEmpty_HeapBecomesEmpty) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(distances[i], i);

    // Act
    heap->clear();

    // Assert
    ASSERT_TRUE(heap->isEmpty());
}

TEST_F(PairingMinHeapTest, EmptyHeapOperations_HeapIsEmpty_ExceptionsThrown) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    unsigned key = 5, id = 10;

    // Assert
    ASSERT_TRUE(heap->isEmpty());
    ASSERT_THROW(heap->deleteMin(), std::out_of_range);
    ASSERT_THROW(heap->decreaseKey(id, key), std::out_of_range);
}

TEST_F(PairingMinHeapTest, InvalidDecreaseKey_NodeDoesNotExistOrNewKeyIsNotSmaller_ExceptionsThrown) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    heap->insertOrUpdate(10, 1);
    unsigned key = 5, id = 2;

    // Act & Assert
    ASSERT_THROW(heap->decreaseKey(id, key), std::out_of_range);
    key = 15;
    ASSERT_THROW(heap->decreaseKey(1, key), std::invalid_argument);
}

TEST_F(PairingMinHeapTest, DecreaseKey_MultipleUpdates_NodeCorrectlyUpdatedAndHeapPropertyMaintained) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(10 + i, i);

    // Act
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->decreaseKey(i, i);

    // Assert
    for (unsigned i = 0; i < NUM_VERTICES; i++) {
        auto min = heap->deleteMin();
        ASSERT_EQ(min, i);
    }
}

TEST_F(PairingMinHeapTest, InsertOrUpdate_ExistingNodeWithDecreasedKey_KeyUpdated) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    heap->insertOrUpdate(20, 1);

    // Act
    heap->insertOrUpdate(10, 1);

    // Assert
    auto min = heap->peek();
    ASSERT_EQ(min, 10);
}

TEST_F(PairingMinHeapTest, DecreaseKey_NewMin_HeapRootUpdated) {
    // Arrange
    heap = new OptimizedKit::PairingMinHeap<unsigned, unsigned>();
    for (unsigned i = 0; i < NUM_VERTICES; i++)
        heap->insertOrUpdate(i+1, i);

    // Act
    heap->decreaseKey(NUM_VERTICES - 1, 0);

    // Assert
    auto minKey = heap->peek();
    ASSERT_EQ(minKey, 0);
    auto minId = heap->deleteMin();
    ASSERT_EQ(minId, NUM_VERTICES - 1);
}
