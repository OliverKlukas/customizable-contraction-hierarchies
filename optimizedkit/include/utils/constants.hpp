#ifndef OPTIMIZEDKIT_CONSTANTS_HPP
#define OPTIMIZEDKIT_CONSTANTS_HPP

#include <limits>

namespace OptimizedKit {
    /**
     * @brief The invalid value used for vertices and edges.
     *
     * @details Using 2^31-1 instead of 2^32-1 avoids 32-bit saturated addition, and this allows the algorithm to
     *          perform saturated addition efficiently by ensuring that the sum of two 2^31-1 values remains within
     *          the bounds of a 32-bit integer.
     *
     * @tparam WeightType - The type of the invalid value.
     */
    template<typename WeightType>
    constexpr auto INFINITY_WEIGHT = static_cast<WeightType>(2147483647u);

    /**
     * @brief The maximum value of any type as defined by std::numeric_limits.
     *
     * @tparam ValueType - The type of the max value.
     */
    template<typename ValueType>
    constexpr auto INVALID_VALUE = std::numeric_limits<ValueType>::max();

}

#endif //OPTIMIZEDKIT_CONSTANTS_HPP
