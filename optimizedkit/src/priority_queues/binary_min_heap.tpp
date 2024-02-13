#include <priority_queues/binary_min_heap.hpp>

template<typename KeyType, typename IdType>
int OptimizedKit::BinaryMinHeap<KeyType, IdType>::getIndex(const IdType &id) const {
    try {
        return indices.at(id);
    } catch (std::out_of_range &e) {
        return -1;
    }
}

template<typename KeyType, typename IdType>
unsigned OptimizedKit::BinaryMinHeap<KeyType, IdType>::getRightChildIndex(unsigned int index) const {
    return 2 * index + 2;
}

template<typename KeyType, typename IdType>
unsigned OptimizedKit::BinaryMinHeap<KeyType, IdType>::getLeftChildIndex(unsigned int index) const {
    return 2 * index + 1;
}

template<typename KeyType, typename IdType>
unsigned OptimizedKit::BinaryMinHeap<KeyType, IdType>::getParentIndex(unsigned int index) const {
    return (index - 1) / 2;
}

template<typename KeyType, typename IdType>
void OptimizedKit::BinaryMinHeap<KeyType, IdType>::siftUp(unsigned int index) {
    while (index != 0 && getParentIndex(index) < heap.size() && heap[getParentIndex(index)].key > heap[index].key) {
        std::swap(heap[index], heap[getParentIndex(index)]);
        indices[heap[index].id] = index;
        indices[heap[getParentIndex(index)].id] = getParentIndex(index);
        index = getParentIndex(index);
    }
}

template<typename KeyType, typename IdType>
void OptimizedKit::BinaryMinHeap<KeyType, IdType>::siftDown(unsigned int index) {
    unsigned int leftChildIndex = getLeftChildIndex(index);
    unsigned int rightChildIndex = getRightChildIndex(index);
    unsigned int smallest = index;
    if (leftChildIndex < heap.size() && heap[leftChildIndex].key < heap[index].key) smallest = leftChildIndex;
    if (rightChildIndex < heap.size() && heap[rightChildIndex].key < heap[smallest].key) smallest = rightChildIndex;
    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        indices[heap[index].id] = index;
        indices[heap[smallest].id] = smallest;
        siftDown(smallest);
    }
}

template<typename KeyType, typename IdType>
void OptimizedKit::BinaryMinHeap<KeyType, IdType>::insertOrUpdate(const KeyType &key) {
    static_assert(std::is_same<KeyType, IdType>::value, "KeyType and IdType must be the same type");
    this->insertOrUpdate(key, key);
}

template<typename KeyType, typename IdType>
void OptimizedKit::BinaryMinHeap<KeyType, IdType>::insertOrUpdate(const KeyType &key, const IdType &id) {
    if (getIndex(id) != -1) {
        decreaseKey(id, key);
        return;
    }
    heap.push_back(BinaryMinHeap::Node{key, id});
    indices[id] = heap.size() - 1;
    siftUp(heap.size() - 1);
}

template<typename KeyType, typename IdType>
IdType OptimizedKit::BinaryMinHeap<KeyType, IdType>::deleteMin() {
    if (heap.empty()) throw std::out_of_range("The heap is empty.");
    Node min = heap[0];
    heap[0] = heap[heap.size() - 1];
    indices[heap[0].id] = 0;
    heap.pop_back();
    indices.erase(min.id);
    siftDown(0);
    return min.id;
}

template<typename KeyType, typename IdType>
KeyType OptimizedKit::BinaryMinHeap<KeyType, IdType>::peek() {
    if (heap.empty()) throw std::out_of_range("The heap is empty.");
    return heap[0].key;
}


template<typename KeyType, typename IdType>
void OptimizedKit::BinaryMinHeap<KeyType, IdType>::decreaseKey(const IdType &id, const KeyType &newKey) {
    unsigned index = getIndex(id);
    if (index == -1) throw std::out_of_range("This vertex id is not in the heap.");
    if (heap[index].key < newKey) throw std::invalid_argument("The new key is greater than the current key.");
    heap[index].key = newKey;
    siftUp(index);
}

template<typename KeyType, typename IdType>
std::string OptimizedKit::BinaryMinHeap<KeyType, IdType>::printOut() const {
    std::string result;
    int level = 0;
    int nodesPerLevel = 1;
    int count = 0;
    while (count < heap.size()) {
        for (int i = 0; i < nodesPerLevel && count < heap.size(); ++i) {
            std::cout << heap[count] << " ";
            ++count;
        }
        std::cout << std::endl;
        level++;
        nodesPerLevel = 1 << level;
    }
    return result;
}

template<typename KeyType, typename IdType>
bool OptimizedKit::BinaryMinHeap<KeyType, IdType>::isEmpty() const {
    return heap.empty() && indices.empty();
}

template<typename KeyType, typename IdType>
void OptimizedKit::BinaryMinHeap<KeyType, IdType>::clear() {
    heap.clear();
    indices.clear();
}
