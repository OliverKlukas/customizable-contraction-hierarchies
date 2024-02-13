#include <priority_queues/pairing_min_heap.hpp>
#include <type_traits>

template<typename KeyType, typename IdType>
void OptimizedKit::PairingMinHeap<KeyType, IdType>::deleteAll(PairingMinHeap::Node *node) {
    if (node == nullptr)
        return;
    deleteAll(node->child);
    deleteAll(node->sibling);
    delete node;
}


template<typename KeyType, typename IdType>
OptimizedKit::PairingMinHeap<KeyType, IdType>::Node *
OptimizedKit::PairingMinHeap<KeyType, IdType>::twoPassMeld(PairingMinHeap::Node *node) {
    if (node == nullptr || node->sibling == nullptr)
        return node;
    Node *nextSibling, *nextNextSibling;
    nextSibling = node->sibling;
    nextNextSibling = nextSibling->sibling;
    node->sibling = nullptr;
    nextSibling->sibling = nullptr;
    return meld(meld(node, nextSibling), twoPassMeld(nextNextSibling));
}

template<typename KeyType, typename IdType>
OptimizedKit::PairingMinHeap<KeyType, IdType>::Node *
OptimizedKit::PairingMinHeap<KeyType, IdType>::meld(PairingMinHeap::Node *first, PairingMinHeap::Node *second) {
    if (first == nullptr)
        return second;
    if (second == nullptr)
        return first;
    if (first->key > second->key)
        std::swap(first, second);
    second->parent = first;
    second->sibling = first->child;
    first->child = second;
    return first;
}

template<typename KeyType, typename IdType>
void OptimizedKit::PairingMinHeap<KeyType, IdType>::decreaseKey(PairingMinHeap::Node *node, const KeyType &newKey) {
    // Update key.
    if(node->key == newKey)
        return;
    if (node->key < newKey)
        throw std::invalid_argument("New key must be smaller than old key");
    node->key = newKey;

    // Remove subtree and meld into heap again if affected node is not root.
    if (node == root)
        return;
    if (node->parent->child == node) {
        node->parent->child = node->sibling;
    } else {
        Node *tmp = node->parent->child;
        while (tmp != nullptr && tmp->sibling != node)
            tmp = tmp->sibling;
        if (tmp != nullptr)
            tmp->sibling = node->sibling;
    }
    node->parent = nullptr;
    node->sibling = nullptr;
    root = meld(root, node);
}

template<typename KeyType, typename IdType>
void OptimizedKit::PairingMinHeap<KeyType, IdType>::decreaseKey(const IdType &id, const KeyType &newKey) {
    auto it = indices.find(id);
    if(it == indices.end())
        throw std::out_of_range("Id not found");
    decreaseKey(it->second, newKey);
}

template<typename KeyType, typename IdType>
bool OptimizedKit::PairingMinHeap<KeyType, IdType>::isEmpty() const {
    return this->root == nullptr;
}

template<typename KeyType, typename IdType>
KeyType OptimizedKit::PairingMinHeap<KeyType, IdType>::peek() {
    return this->root->key;
}

template<typename KeyType, typename IdType>
void OptimizedKit::PairingMinHeap<KeyType, IdType>::clear() {
    deleteAll(this->root);
    root = nullptr;
    indices.clear();
}

template<typename KeyType, typename IdType>
IdType OptimizedKit::PairingMinHeap<KeyType, IdType>::deleteMin() {
    if(this->root == nullptr)
        throw std::out_of_range("Heap is empty");
    Node *min = this->root;
    indices.erase(min->id);
    IdType minId = min->id;
    root = twoPassMeld(this->root->child);
    delete min;
    return minId;
}

template<typename KeyType, typename IdType>
void OptimizedKit::PairingMinHeap<KeyType, IdType>::insertOrUpdate(const KeyType &key) {
    static_assert(std::is_same<KeyType, IdType>::value, "KeyType and IdType must be the same type");
    auto it = indices.find(key);
    if (it != indices.end())
        return;
    Node *node = new Node(key, key);
    this->root = meld(this->root, node);
    indices[key] = node;
}

template<typename KeyType, typename IdType>
void OptimizedKit::PairingMinHeap<KeyType, IdType>::insertOrUpdate(const KeyType &key, const IdType &id) {
    auto it = indices.find(id);
    if (it != indices.end()) {
        auto node = it->second;
        if(node->key == key)
            return;
        decreaseKey(it->second, key);
        return;
    }
    Node *node = new Node(key, id);
    this->root = meld(this->root, node);
    indices[id] = node;
}
