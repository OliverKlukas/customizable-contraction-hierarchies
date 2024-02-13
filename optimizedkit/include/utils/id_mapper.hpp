#ifndef OPTIMIZEDKIT_ID_MAPPER_HPP
#define OPTIMIZEDKIT_ID_MAPPER_HPP

#include <vector>
#include <cassert>
#include "types.hpp"
#include "vector_helper.hpp"

namespace OptimizedKit {
    /**
     * @brief Maps global ids to local ids via a filter.
     */
    class IdMapper {
    public:
        IdMapper() = default;

        /**
         * @brief Constructs an IdMapper that maps global ids to local ids and vice versa.
         *
         * @param filter - The filter indicating which local ids are also global ids.
         */
        explicit IdMapper(const Filter &filter);

        /**
         * @brief Maps the given global id to a local id.
         *
         * @param globalId - The global id to map.
         * @return Returns the local id of the given global id.
         */
        [[nodiscard]] unsigned toLocal(unsigned globalId) const;

        /**
         * @brief Returns the number of local ids.
         *
         * @return Returns the number of local ids.
         */
        [[nodiscard]] unsigned getLocalIdCount() const {
            return localIdCount;
        }

        /**
         * @brief Removes ids from mapping.
         *
         * @param filter - Filter indicating ids to remove.
         */
        void remove(const Filter &filter);

    private:
        std::vector<unsigned> mapping;
        unsigned localIdCount{};
    };
}

#endif //OPTIMIZEDKIT_ID_MAPPER_HPP
