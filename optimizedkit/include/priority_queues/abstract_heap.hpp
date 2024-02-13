#ifndef OPTIMIZEDKIT_ABSTRACT_HEAP_HPP
#define OPTIMIZEDKIT_ABSTRACT_HEAP_HPP

namespace OptimizedKit{
    template<typename KeyType, typename IdType>
    class AbstractHeap{
    public:
        virtual void insertOrUpdate(const KeyType &key, const IdType &id) = 0;

        virtual ~AbstractHeap() = default;

        virtual void insertOrUpdate(const KeyType &key) = 0;

        virtual IdType deleteMin() = 0;

        virtual KeyType peek() = 0;

        virtual void decreaseKey(const IdType &id, const KeyType &newKey) = 0;

        virtual void clear() = 0;

        [[nodiscard]] virtual bool isEmpty() const = 0;

        [[nodiscard]] virtual int size() const = 0;

    };
}

#endif //OPTIMIZEDKIT_ABSTRACT_HEAP_HPP
