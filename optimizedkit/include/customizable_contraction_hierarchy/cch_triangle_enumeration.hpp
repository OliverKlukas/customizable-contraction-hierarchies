#ifndef OPTIMIZEDKIT_CCH_TRIANGLE_ENUMERATION_HPP
#define OPTIMIZEDKIT_CCH_TRIANGLE_ENUMERATION_HPP

#include "utils/types.hpp"
#include "cch_preprocessor.hpp"

namespace OptimizedKit {
    template<class F>
    void
    enumerateLowerTriangles(const CchPreprocessor &preprocessor, EdgeId bc, const F &f);

    template<class F>
    void
    enumerateIntermediateTriangles(const CchPreprocessor &preprocessor, EdgeId ac, const F &f);

    template<class F>
    void
    enumerateUpperTriangles(const CchPreprocessor &preprocessor, EdgeId ab, const F &f);
}

#include "../../src/customizable_contraction_hierarchy/cch_triangle_enumeration.tpp"

#endif //OPTIMIZEDKIT_CCH_TRIANGLE_ENUMERATION_HPP
