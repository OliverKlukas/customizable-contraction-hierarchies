#ifndef OPTIMIZEDKIT_PERMUTATION_HPP
#define OPTIMIZEDKIT_PERMUTATION_HPP

#include <vector>
#include <cassert>
#include "vector_helper.hpp"
#include "utils/types.hpp"

namespace OptimizedKit {
    /**
     * @brief Verifies that permutation does not contains duplicates and contains all numbers from 0 to permutation.size() - 1.
     *
     * @copyright Inspired by RoutingKit's is_permutation in permutation.h.
     *
     * @tparam P - Permutation type.
     * @param p - Permutation vector.
     * @return Returns true if permutation is state, false otherwise.
     */
    template<class P>
    bool isPermutation(const std::vector<P> &p);

    /**
     * @brief Applies permutation p to vector v resulting in {v[p[0]], v[p[1]], v[p[2]], ... , v[p[n]]}.
     *
     * @copyright Inspired by RoutingKit's apply_permutation in permutation.h.
     *
     * @tparam P - Permutation type.
     * @tparam V - Vector type.
     * @param p - Permutation vector.
     * @param v - Value vector.
     * @return Returns vector resulting from applying permutation p to vector v.
     */
    template<class P, class V>
    std::vector<V> applyPermutation(const std::vector<P> &p, const std::vector<V> &v);

    /**
     * @brief Applies permutation p to vector v in place resulting in {v[p[0]], v[p[1]], v[p[2]], ... , v[p[n]]}.
     *
     * @copyright Inspired by RoutingKit's apply_permutation in permutation.h.
     *
     * @tparam P - Permutation type.
     * @tparam V - Vector type.
     * @param p - Permutation vector.
     * @param v - Value vector.
     * @return Returns vector resulting from applying permutation p to vector v.
     */
    template<class P, class V>
    std::vector<V> applyPermutation(const std::vector<P> &p, std::vector<V> &&v);

    /**
     * @brief Inverts permutation p resulting in {p^-1[0], p^-1[1], p^-1[2], ... , p^-1[n]}.
     *
     * @copyright Inspired by RoutingKit's invert_permutation in permutation.h.
     *
     * @tparam P - Permutation type.
     * @param p - Permutation vector.
     * @return Returns inverted permutation.
     */
    template<class P>
    std::vector<P> invertPermutation(const std::vector<P> &p);

    /**
     * @brief Applies a permutation to the elements of a vector in place. Meaning that the resulting vector is {p[v[0]],
     *        p[v[1]], p[v[2]], ... , p[v[n]]}.
     *
     * @copyright Inspired by RoutingKit's inplace_apply_permutation_to_elements_of in permutation.h.
     *
     * @param p - Permutation vector.
     * @param v - Vector to apply permutation to.
     */
    void inplaceApplyPermutationToElementsOf(const std::vector<unsigned> &p, std::vector<unsigned> &v);

    /**
     * @brief Applies a permutation to the elements of a vector. Meaning that the resulting vector is {p[v[0]], p[v[1]],
     *        p[v[2]], ... , p[v[n]]}.
     *
     * @copyright Inspired by RoutingKit's apply_permutation_to_elements_of permutation.h.
     *
     * @param p - Permutation vector.
     * @param v - Value vector.
     * @return Returns vector resulting from applying permutation p to vector v.
     */
    std::vector<unsigned> applyPermutationToElementsOf(const std::vector<unsigned> &p, const std::vector<unsigned> &v);

    /**
     * @brief Applies permutation p in inverse order to vector v resulting in {v[p^-1[n]], v[p^-1[n-1]], v[p^-1[n-2]],
     *        ... , v[p^-1[0]]}. Copies the values from v to the resulting vector.
     *
     * @copyright Inspired by RoutingKit's apply_inverse_permutation in permutation.h.
     *
     * @tparam P - Permutation type.
     * @tparam V - Vector type.
     * @param p - Permutation vector.
     * @param v - Value vector.
     * @return Returns vector resulting from applying permutation p in inverse order to vector v.
     */
    template<class P, class V>
    std::vector<V> applyInversePermutation(const std::vector<P> &p, const std::vector<V> &v);

    /**
     * @brief Applies permutation p in inverse order to vector v resulting in {v[p^-1[n]], v[p^-1[n-1]], v[p^-1[n-2]],
     *       ... , v[p^-1[0]]}. Moves the values from v to the resulting vector.
     *
     * @copyright Inspired by RoutingKit's apply_inverse_permutation in permutation.h.
     *
     * @tparam P - Permutation type.
     * @tparam V - Vector type.
     * @param p - Permutation vector.
     * @param v - Value vector.
     * @return Returns vector resulting from applying permutation p in inverse order to vector v.
     */
    template<class P, class V>
    std::vector<V> applyInversePermutation(const std::vector<P> &p, std::vector<V> &&v);

    /**
     * @brief Chain two permutations p and q such that the resulting permutation is p(q(x)).
     *
     * @copyright Inspired by RoutingKit's chain_permutation_first_left_then_right in permutation.h.
     *
     * @param p Permutation.
     * @param q Permutation.
     * @return Returns the chained permutation.
     */
    std::vector<unsigned>
    chainPermutationFirstLeftThenRight(const std::vector<unsigned> &p, const std::vector<unsigned> &q);

    /**
     * @brief Compute the inverse sort permutation first by tail then by head and apply sort to tail.
     *
     * @copyright Inspired by RoutingKit's compute_inverse_sort_permutation_first_by_tail_then_by_head_and_apply_sort_to_tail in graph_util.h.
     *
     * @param tail - tail of the graph.
     * @param head - head of the graph.
     * @return Returns the sorted permutation.
     */
    std::vector<unsigned> computeInverseSortPermutationFirstByTailThenByHeadAndApplySortToTail(
            std::vector<VertexId> &tail, const std::vector<VertexId> &head);

    /**
     * @brief Compute the sort permutation first by tail then by head and apply sort to tail.
     *
     * @copyright Inspired by RoutingKit's compute_sort_permutation_first_by_tail_then_by_head_and_apply_sort_to_tail in graph_util.h.
     *
     * @param tail - tail of the graph.
     * @param head - head of the graph.
     * @return Returns the sorted permutation.
     */
    std::vector<unsigned>
    computeSortPermutationFirstByTailThenByHeadAndApplySortToTail(std::vector<VertexId> &tail,
                                                                  const std::vector<VertexId> &head);

    /**
     * @brief Computes an identity permutation of size n.
     *
     * @copyright Inspired by RoutingKit's identity_permutation in permutation.h.
     *
     * @param n - Size of the permutation.
     * @tparam P - Permutation type.
     * @return Returns a vector containing per index its index, i.e. {0, 1, 2, ... , n-1}.
     */
    template<class P>
    std::vector<P> identityPermutation(unsigned n);

    /**
     * @brief Computes a stable sort permutation using key.
     *
     * @example Given v = {3, 1, 0, 2} then the permutation vector is {2, 1, 3, 0}.
     *
     * @tparam V - Type of the vector.
     * @param v - Vector to sort.
     * @return Returns the stable sort permutation.
     */
    template<class V>
    std::vector<unsigned> computeStableSortPermutation(const std::vector<V> &v);

    /**
     * @brief Computes an inverse stable sort permutation using key.
     *
     * @example Given v = {3, 1, 0, 2}, then the permutation vector is {3, 1, 0, 2}.
     *
     * @tparam V - Type of the vector.
     * @param v - Vector to sort.
     * @return Returns the inverse stable sort permutation.
     */
    template<class V>
    std::vector<unsigned> computeInverseStableSortPermutation(const std::vector<V> &v);
}

#include "../../src/utils/permutation.tpp"

#endif //OPTIMIZEDKIT_PERMUTATION_HPP


