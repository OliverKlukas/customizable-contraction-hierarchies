#ifndef OPTIMIZEDKIT_PAIRING_MIN_HEAP_HPP
#define OPTIMIZEDKIT_PAIRING_MIN_HEAP_HPP

#include <unordered_map>
#include "abstract_heap.hpp"

namespace OptimizedKit {
    template<typename KeyType, typename IdType>
    class PairingMinHeap : public AbstractHeap<KeyType, IdType> {
    public:
        struct Node {
            KeyType key;
            IdType id;
            Node *parent, *child, *sibling;

            Node(KeyType key, IdType id) : key(key), id(id), parent(nullptr), child(nullptr), sibling(nullptr) {}
        };

        PairingMinHeap() = default;

        ~PairingMinHeap(){
            clear();
        }

        void insertOrUpdate(const KeyType &key, const IdType &id);

        void insertOrUpdate(const KeyType &key);

        IdType deleteMin();

        KeyType peek();

        void decreaseKey(const IdType &id, const KeyType &newKey);

        void clear();

        [[nodiscard]] bool isEmpty() const;

        [[nodiscard]] int size() const { return indices.size(); }

    private:
        Node *root;
        std::unordered_map<IdType, Node *> indices;

        Node *meld(Node *first, Node *second);

        Node *twoPassMeld(Node *node);

        void decreaseKey(Node *node, const KeyType &newKey);

        void deleteAll(Node *node);
    };

}

#include "../../src/priority_queues/pairing_min_heap.tpp"

#endif //OPTIMIZEDKIT_PAIRING_MIN_HEAP_HPP
