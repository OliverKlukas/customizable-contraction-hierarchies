#ifndef OPTIMIZEDKIT_BINARY_MIN_HEAP_HPP
#define OPTIMIZEDKIT_BINARY_MIN_HEAP_HPP

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "abstract_heap.hpp"

namespace OptimizedKit {

    /**
     * @brief A Binary Min-Heap implementation.
     *
     * @details If applied to a graph, the heap's nodes represent the vertices of the graph (= node id) and a distance
     *          between two vertices (= node key).
     *
     * @tparam KeyType - The type of the keys stored in the heap, default is unsigned.
     * @tparam IdType - The type of the ids stored in the heap, default is unsigned.
     */
    template<typename KeyType, typename IdType>
    class BinaryMinHeap : public AbstractHeap<KeyType, IdType> {
    public:
        /**
         * @brief A node in the binary min heap.
         *
         * @details The node contains a key and an id. The key is used to determine the position of the node in the
         *          heap. The id is used to identify a node.
         */
        struct Node {
            KeyType key;
            IdType id;

            /**
             * @brief Overloads the << operator to print a node.
             *
             * @param os - The output stream.
             * @param node - The node to print.
             * @return Returns the output stream with the node printed.
             */
            friend std::ostream &operator<<(std::ostream &os, const Node &node) {
                os << "(" << node.key << ", " << node.id << ")";
                return os;
            }
        };

        /**
         * @brief Constructs a new empty Binary Min-Heap object.
         */
        BinaryMinHeap() = default;

        /**
         * @brief Destructs a Binary Min-Heap object.
         */
        ~BinaryMinHeap() = default;

        /**
         * @brief Inserts a new node into the heap.
         *
         * @param key - The key of the new node.
         * @param id - The id of the new node.
         */
        void insertOrUpdate(const KeyType &key, const IdType &id);

        /**
         * @brief Inserts a new node into the heap. Only works if the key and id are of the same type.
         *
         * @param key - The key and id of the new node.
         */
        void insertOrUpdate(const KeyType &key);

        /**
         * @brief Returns the node with the minimum key in the heap.
         *
         * @return Returns the node with the minimum key in the heap.
         * @throws std::out_of_range - If the heap is empty.
         */
        IdType deleteMin();

        /**
         * @brief Returns the node with the minimum key in the heap without removing it.
         *
         * @return Returns the node with the minimum key in the heap.
         * @throws std::out_of_range - If the heap is empty.
         */
        KeyType peek();

        /**
         * @brief Retrieve iterator to beginning.
         *
         * @return Returns an iterator to the beginning of the heap.
         */
        auto begin() {
            return heap.begin();
        }

        /**
         * @brief Retrieve iterator to end.
         *
         * @return Returns an iterator to the end of the heap.
         */
        auto end() {
            return heap.end();
        }

        /**
         * @brief Decreases the key of a node to a new smaller key.
         *
         * @param id - The id of the node to decrease its key.
         * @param newKey - The new key of the node, needs to be smaller than the existing one.
         * @throws std::out_of_range - If the id is not in the heap.
         * @throws std::invalid_argument - If the new key is not smaller than the existing one.
         */
        void decreaseKey(const IdType &id, const KeyType &newKey);

        /**
         * @brief Representation of the current heap state.
         *
         * @return - Returns a string representation of the heap.
         */
        [[nodiscard]] std::string printOut() const;

        /**
         * @brief Clears the heap by removing all nodes.
         */
        void clear();

        /**
         * @brief Checks if the heap is empty.
         *
         * @return Returns true if the heap is empty, false otherwise.
         */
        [[nodiscard]] bool isEmpty() const;

        /**
         * @brief Size of the heap.
         *
         * @return Returns the number of nodes in the heap
         */
        [[nodiscard]] int size() const { return heap.size(); }

//    private:
        std::vector<Node> heap;

        std::unordered_map<IdType, unsigned> indices;

        int getIndex(const IdType &id) const;

        void siftUp(unsigned index);

        void siftDown(unsigned index);

        [[nodiscard]] unsigned getParentIndex(unsigned index) const;

        [[nodiscard]] unsigned getLeftChildIndex(unsigned index) const;

        [[nodiscard]] unsigned getRightChildIndex(unsigned index) const;
    };

}

#include "../../src/priority_queues/binary_min_heap.tpp"

#endif //OPTIMIZEDKIT_BINARY_MIN_HEAP_HPP
