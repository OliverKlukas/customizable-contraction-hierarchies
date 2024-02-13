#include <utils/permutation.hpp>

template<class P>
bool OptimizedKit::isPermutation(const std::vector<P> &p) {
    std::vector<bool> found(p.size(), false);
    for (unsigned x: p) {
        if (x >= p.size())
            return false;
        if (found[x])
            return false;
        found[x] = true;
    }
    return true;
}

template<class P, class V>
std::vector<V> OptimizedKit::applyPermutation(const std::vector<P> &p, const std::vector<V> &v) {
    assert(isPermutation(p) && "p must be a permutation");
    assert(p.size() == v.size() && "permutation and vector must have the same size");
    std::vector<V> r(v.size());
    for (unsigned i = 0; i < v.size(); ++i)
        r[i] = v[p[i]];
    return r;
}

template<class P, class V>
std::vector<V> OptimizedKit::applyPermutation(const std::vector<P> &p, std::vector<V> &&v) {
    assert(isPermutation(p) && "p must be a permutation");
    assert(p.size() == v.size() && "permutation and vector must have the same size");
    std::vector<V> r(v.size());
    for (P i = 0; i < v.size(); ++i)
        r[i] = std::move(v[p[i]]);
    return r;
}

template<class P>
std::vector<P> OptimizedKit::invertPermutation(const std::vector<P> &p) {
    assert(isPermutation(p) && "p must be a permutation");
    std::vector<unsigned> invP(p.size());
    for (unsigned i = 0; i < p.size(); ++i)
        invP[p[i]] = i;
    return invP;
}

template<class P, class V>
std::vector<V> OptimizedKit::applyInversePermutation(const std::vector<P> &p, const std::vector<V> &v) {
    assert(isPermutation(p) && "p must be a permutation");
    assert(p.size() == v.size() && "permutation and vector must have the same size");
    std::vector<V> r(v.size());
    for (P i = 0; i < v.size(); ++i)
        r[p[i]] = v[i];
    return r;
}

template<class P, class V>
std::vector<V> OptimizedKit::applyInversePermutation(const std::vector<P> &p, std::vector<V> &&v) {
    assert(isPermutation(p) && "p must be a permutation");
    assert(p.size() == v.size() && "permutation and vector must have the same size");
    std::vector<V> r(v.size());
    for (P i = 0; i < v.size(); ++i)
        r[p[i]] = std::move(v[i]);
    return r;
}

template<class P>
std::vector<P> OptimizedKit::identityPermutation(unsigned int n) {
    std::vector<P> p(n);
    for (P i = 0; i < n; ++i)
        p[i] = i;
    return p;
}

template<class V>
std::vector<unsigned>
OptimizedKit::computeStableSortPermutation(const std::vector<V> &v) {
    std::vector <unsigned> p(v.size());
    for (unsigned i = 0; i < v.size(); ++i)
        p[i] = i;
    std::stable_sort(p.begin(), p.end(), [&](unsigned i, unsigned j) { return v[i] < v[j]; });
    return p;
}

template<class V>
std::vector<unsigned>
OptimizedKit::computeInverseStableSortPermutation(const std::vector<V> &v) {
    return invertPermutation(computeStableSortPermutation(v));
}
