#include "utils/permutation.hpp"

std::vector<unsigned> OptimizedKit::computeInverseSortPermutationFirstByTailThenByHeadAndApplySortToTail(
        std::vector<VertexId> &tail,
        const std::vector<VertexId> &head
) {
    auto p = computeInverseStableSortPermutation(head);
    tail = applyInversePermutation(p, std::move(tail));
    auto q = computeInverseStableSortPermutation(tail);
    tail = applyInversePermutation(q, std::move(tail));
    assert(isVectorSorted(tail));
    return chainPermutationFirstLeftThenRight(q, p);
}

std::vector<unsigned> OptimizedKit::computeSortPermutationFirstByTailThenByHeadAndApplySortToTail(
        std::vector<VertexId> &tail,
        const std::vector<VertexId> &head
) {
    auto p = computeStableSortPermutation(head);
    tail = applyPermutation(p, std::move(tail));
    auto q = computeStableSortPermutation(tail);
    tail = applyPermutation(q, std::move(tail));
    assert(isVectorSorted(tail));
    return chainPermutationFirstLeftThenRight(p, q);
}

void OptimizedKit::inplaceApplyPermutationToElementsOf(const std::vector<unsigned> &p, std::vector<unsigned> &v) {
    assert(isPermutation(p) && "p must be a permutation");
    assert(std::all_of(v.begin(), v.end(), [&](unsigned x) { return x < p.size(); }) &&
           "v has an out of bounds element");
    for (unsigned int &i: v)
        i = p[i];
}

std::vector<unsigned>
OptimizedKit::applyPermutationToElementsOf(const std::vector<unsigned> &p, const std::vector<unsigned> &v) {
    assert(isPermutation(p) && "p must be a permutation");
    assert(std::all_of(v.begin(), v.end(), [&](unsigned x) { return x < p.size(); }) && "v has an out of bounds element");
    std::vector<unsigned> r = v;
    inplaceApplyPermutationToElementsOf(p, r);
    return r;
}

std::vector<unsigned>
OptimizedKit::chainPermutationFirstLeftThenRight(const std::vector<unsigned> &p, const std::vector<unsigned> &q) {
    assert(isPermutation(p) && "p must be a permutation");
    assert(isPermutation(q) && "q must be a permutation");
    assert(p.size() == q.size() && "p and q must permute the same number of objects");
    std::vector<unsigned> r(p.size());
    for (unsigned i = 0; i < r.size(); ++i)
        r[i] = p[q[i]];
    return r;
}
