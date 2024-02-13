#include <customizable_contraction_hierarchy/cch_triangle_enumeration.hpp>

template<class F>
void OptimizedKit::enumerateUpperTriangles(const CchPreprocessor &preprocessor, EdgeId ab, const F &f) {
    VertexId a = preprocessor.upwardsGraph.tail[ab];
    VertexId b = preprocessor.upwardsGraph.head[ab];

    EdgeId aUp = ab + 1;
    EdgeId bUp = preprocessor.upwardsGraph.adjacencyIndices[b];

    while (aUp != preprocessor.upwardsGraph.adjacencyIndices[a + 1] && bUp != preprocessor.upwardsGraph.adjacencyIndices[b + 1]) {
        if (preprocessor.upwardsGraph.head[aUp] < preprocessor.upwardsGraph.head[bUp]) {
            ++aUp;
        } else if (preprocessor.upwardsGraph.head[aUp] > preprocessor.upwardsGraph.head[bUp]) {
            ++bUp;
        } else {
            VertexId c = preprocessor.upwardsGraph.head[aUp];
            EdgeId ac = aUp;
            EdgeId bc = bUp;
            f(ab, ac, bc, a, b, c);
            ++aUp;
            ++bUp;
        }
    }
}

template<class F>
void OptimizedKit::enumerateIntermediateTriangles(const CchPreprocessor &preprocessor, EdgeId ac, const F &f) {
    VertexId a = preprocessor.upwardsGraph.tail[ac];
    VertexId c = preprocessor.upwardsGraph.head[ac];

    EdgeId aUp = preprocessor.upwardsGraph.adjacencyIndices[a];
    EdgeId cDown = preprocessor.downwardsGraph.adjacencyIndices[c];

    while (aUp != ac && cDown != preprocessor.downwardsGraph.adjacencyIndices[c + 1]) {
        if (preprocessor.upwardsGraph.head[aUp] < preprocessor.downwardsGraph.head[cDown]) {
            ++aUp;
        } else if (preprocessor.upwardsGraph.head[aUp] > preprocessor.downwardsGraph.head[cDown]) {
            ++cDown;
        } else {
            VertexId b = preprocessor.upwardsGraph.head[aUp];
            EdgeId ab = aUp;
            EdgeId bc = preprocessor.downwardsToUpwardsGraph[cDown];
            f(ab, ac, bc, a, b, c);
            ++aUp;
            ++cDown;
        }
    }
}

template<class F>
void OptimizedKit::enumerateLowerTriangles(const CchPreprocessor &preprocessor, EdgeId bc, const F &f) {
    VertexId b = preprocessor.upwardsGraph.tail[bc];
    VertexId c = preprocessor.upwardsGraph.head[bc];

    EdgeId bDown = preprocessor.downwardsGraph.adjacencyIndices[b];
    EdgeId cDown = preprocessor.downwardsGraph.adjacencyIndices[c];

    while (bDown != preprocessor.downwardsGraph.adjacencyIndices[b + 1] && cDown != preprocessor.downwardsGraph.adjacencyIndices[c + 1]) {
        if (preprocessor.downwardsGraph.head[bDown] < preprocessor.downwardsGraph.head[cDown]) {
            ++bDown;
        } else if (preprocessor.downwardsGraph.head[bDown] > preprocessor.downwardsGraph.head[cDown]) {
            ++cDown;
        } else {
            VertexId a = preprocessor.downwardsGraph.head[bDown];
            EdgeId ab = preprocessor.downwardsToUpwardsGraph[bDown];
            EdgeId ac = preprocessor.downwardsToUpwardsGraph[cDown];
            f(ab, ac, bc, a, b, c);
            ++bDown;
            ++cDown;
        }
    }
}
