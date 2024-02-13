#ifndef OPTIMIZEDKIT_ENUMS_HPP
#define OPTIMIZEDKIT_ENUMS_HPP

namespace OptimizedKit {
    /**
     * @brief The state of the CCH query object.
     */
    enum class QueryState {
        UNINITIALIZED,
        INITIALIZED,
        FINISHED
    };

    /**
     * @brief The direction of a bidirectional search.
     */
    enum class SearchDirection {
        FORWARDS,
        BACKWARDS
    };

    /**
     * @brief The state of the CCH customizer object.
     */
    enum class CustomizerState {
        UNCUSTOMIZED,
        BASE_CUSTOMIZED,
        PERFECT_CUSTOMIZED
    };

    /**
     * @brief The type of the heap used.
     */
    enum class HeapType {
        BINARY,
        PAIRING
    };
}

#endif //OPTIMIZEDKIT_ENUMS_HPP

