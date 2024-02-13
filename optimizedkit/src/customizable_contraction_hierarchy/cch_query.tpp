#include <customizable_contraction_hierarchy/cch_query.hpp>

template<typename WeightType>
OptimizedKit::CchQuery<WeightType>::CchQuery(const CchCustomizer <WeightType> &customizer, HeapType heapType)
        : state(QueryState::INITIALIZED), cchCustomizer(&customizer), cchPreprocessor(customizer.cchPreprocessor),
          cchGraph(cchPreprocessor, cchCustomizer), biDirectionalDijkstra(cchGraph, heapType),
          globalSource(INVALID_VALUE < VertexId > ), globalTarget(INVALID_VALUE < VertexId > ),
          localSource(INVALID_VALUE < VertexId > ), localTarget(INVALID_VALUE < VertexId > ) {}

template<typename WeightType>
OptimizedKit::CchQuery<WeightType> &
OptimizedKit::CchQuery<WeightType>::reset(const CchCustomizer <WeightType> &customizer) {
    state = QueryState::UNINITIALIZED;
    cchCustomizer = &customizer;
    cchPreprocessor = cchCustomizer->cchPreprocessor;
    cchGraph = CchGraph(cchPreprocessor, cchCustomizer);
    biDirectionalDijkstra = BiDirectionalDijkstra(cchGraph);
    globalSource = INVALID_VALUE<VertexId>;
    globalTarget = INVALID_VALUE<VertexId>;
    localSource = INVALID_VALUE<VertexId>;
    localTarget = INVALID_VALUE<VertexId>;
    vertexPath.clear();
    edgePath.clear();
    state = QueryState::INITIALIZED;
    return *this;
}

template<typename WeightType>
OptimizedKit::CchQuery<WeightType> &
OptimizedKit::CchQuery<WeightType>::run(OptimizedKit::VertexId source, OptimizedKit::VertexId target, bool debug) {
    assert(state == QueryState::INITIALIZED || state == QueryState::FINISHED);
    assert(source < cchPreprocessor->rank.size() && "Source vertex id is out of bounds.");
    assert(target < cchPreprocessor->rank.size() && "Target vertex id is out of bounds.");
    globalSource = source;
    globalTarget = target;
    localSource = cchPreprocessor->rank[source];
    localTarget = cchPreprocessor->rank[target];
    vertexPath.clear();
    edgePath.clear();
    if (localSource == localTarget) {
        state = QueryState::FINISHED;
        return *this;
    }
    biDirectionalDijkstra.run(localSource, localTarget, debug);
    state = QueryState::FINISHED;
    return *this;
}

template<typename WeightType>
OptimizedKit::EdgeId OptimizedKit::CchQuery<WeightType>::unpackForwardOriginalEdge(EdgeId cchEdge) {
    auto i = cchPreprocessor->doesCchEdgeHaveInputEdgeMapper.toLocal(cchEdge);

    if (cchPreprocessor->forwardInputEdgeOfCchEdge[i] == INVALID_VALUE < VertexId >)
        return INVALID_VALUE<EdgeId>;

    EdgeId originalEdge = cchPreprocessor->forwardInputEdgeOfCchEdge[i];
    if (cchCustomizer->forwardWeights[cchEdge] == cchCustomizer->inputWeights[originalEdge])
        return originalEdge;

    if (cchPreprocessor->doesCchEdgeHaveExtraInputEdge[cchEdge]) {
        auto j = cchPreprocessor->doesCchEdgeHaveExtraInputEdgeMapper.toLocal(cchEdge);
        for (auto k = cchPreprocessor->extraForwardInputEdgeOfCchAdjacencyEdges[j];
             k < cchPreprocessor->extraForwardInputEdgeOfCchAdjacencyEdges[j + 1]; ++k) {
            originalEdge = cchPreprocessor->extraForwardInputEdgeOfCch[k];
            if (cchCustomizer->forwardWeights[cchEdge] == cchCustomizer->inputWeights[originalEdge])
                return originalEdge;
        }
    }
    return INVALID_VALUE<EdgeId>;
}

template<typename WeightType>
OptimizedKit::EdgeId OptimizedKit::CchQuery<WeightType>::unpackBackwardOriginalEdge(EdgeId cchEdge) {
    auto i = cchPreprocessor->doesCchEdgeHaveInputEdgeMapper.toLocal(cchEdge);

    if (cchPreprocessor->backwardInputEdgeOfCchEdge[i] == INVALID_VALUE < VertexId >)
        return INVALID_VALUE<EdgeId>;

    EdgeId originalEdge = cchPreprocessor->backwardInputEdgeOfCchEdge[i];
    if (cchCustomizer->backwardWeights[cchEdge] == cchCustomizer->inputWeights[originalEdge])
        return originalEdge;

    if (cchPreprocessor->doesCchEdgeHaveExtraInputEdge[cchEdge]) {
        auto j = cchPreprocessor->doesCchEdgeHaveExtraInputEdgeMapper.toLocal(cchEdge);
        for (auto k = cchPreprocessor->extraBackwardInputEdgeOfCchAdjacencyEdges[j];
             k < cchPreprocessor->extraBackwardInputEdgeOfCchAdjacencyEdges[j + 1]; ++k) {
            originalEdge = cchPreprocessor->extraBackwardInputEdgeOfCch[k];
            if (cchCustomizer->backwardWeights[cchEdge] == cchCustomizer->inputWeights[originalEdge])
                return originalEdge;
        }
    }
    return INVALID_VALUE<EdgeId>;
}

template<typename WeightType>
OptimizedKit::EdgeId OptimizedKit::CchQuery<WeightType>::unpackOriginalEdge(EdgeId cchEdge, bool forward) {
    if (!cchPreprocessor->doesCchEdgeHaveInputEdge[cchEdge])
        return INVALID_VALUE<EdgeId>;
    if (forward)
        return unpackForwardOriginalEdge(cchEdge);
    else
        return unpackBackwardOriginalEdge(cchEdge);
}

template<typename WeightType>
template<class OnMissingFound>
void OptimizedKit::CchQuery<WeightType>::unpackLowerTriangles(bool forward, VertexId b, VertexId c, EdgeId bc,
                                                              const OnMissingFound &onMissingFound) {
    assert(bc != INVALID_VALUE < EdgeId >);
    assert(b == cchPreprocessor->upwardsGraph.tail[bc]);
    assert(c == cchPreprocessor->upwardsGraph.head[bc]);

    // Identify if the triangle was relaxed used in the shortest distance computation.
    VertexId a = INVALID_VALUE<VertexId>;
    EdgeId ab = INVALID_VALUE<EdgeId>;
    EdgeId ac = INVALID_VALUE<EdgeId>;
    auto unpacker = [&](EdgeId ab_, EdgeId ac_, EdgeId bc_, VertexId a_, VertexId b_, VertexId c_) {
        if ((forward
                && cchCustomizer->forwardWeights[bc_] ==
                    cchCustomizer->backwardWeights[ab_] + cchCustomizer->forwardWeights[ac_])
            || (!forward
                && cchCustomizer->backwardWeights[bc_] ==
                    cchCustomizer->forwardWeights[ab_] + cchCustomizer->backwardWeights[ac_])) {
            a = a_;
            ab = ab_;
            ac = ac_;
        }
    };

    // Identify possible lower triangles between bc.
    enumerateLowerTriangles(*cchPreprocessor, bc, unpacker);

    // Breaking condition of recursion.
    if (a == INVALID_VALUE < VertexId >) {
        onMissingFound(forward ? b : c, bc, forward);
        return;
    }

    // Recursively identify the next lower triangles with missing vertices.
    unpackLowerTriangles(!forward, a, forward ? b : c, forward ? ab : ac, onMissingFound);
    unpackLowerTriangles(forward, a, forward ? c : b, forward ? ac : ab, onMissingFound);
}

template<typename WeightType>
std::vector<OptimizedKit::EdgeId> OptimizedKit::CchQuery<WeightType>::getEdgePath() {
    assert(state == QueryState::FINISHED);
    edgePath.clear();

    // Check success of Dijkstra.
    if (getQueryWeight() == INFINITY_WEIGHT < WeightType > ||
        biDirectionalDijkstra.meetingVertex == INVALID_VALUE < VertexId >) {
        return edgePath;
    }

    auto onMissingEdgeFound = [&](VertexId cchVertex, EdgeId cchEdge, bool forward) {
        (void) cchVertex;
        EdgeId edge = unpackOriginalEdge(cchEdge, forward);
        assert(edge != INVALID_VALUE < EdgeId >);
        edgePath.push_back(edge);
    };

    // Path source -> meeting node.
    {
        std::vector<VertexId> forwardVertexPath;
        VertexId x = biDirectionalDijkstra.meetingVertex;
        while (x != localSource) {
            forwardVertexPath.push_back(x);
            x = biDirectionalDijkstra.forwardPredecessor[x];
            assert(x != INVALID_VALUE < VertexId > && "Invalid predecessor found.");
        }
        forwardVertexPath.push_back(localSource);

        // Convert local vertex id path to global edge id path, source -> meeting point.
        for (unsigned i = forwardVertexPath.size() - 1; i != 0; --i) {
            unpackLowerTriangles(true, forwardVertexPath[i], forwardVertexPath[i - 1],
                                 findEdge(cchPreprocessor->upwardsGraph.adjacencyIndices,
                                          cchPreprocessor->upwardsGraph.head, forwardVertexPath[i],
                                          forwardVertexPath[i - 1]), onMissingEdgeFound);
        }
    }

    // Path meeting node -> target.
    {
        VertexId c = biDirectionalDijkstra.meetingVertex;
        if (c == localTarget)
            return edgePath;
        VertexId b = biDirectionalDijkstra.backwardPredecessor[c];
        assert(b != INVALID_VALUE < VertexId > && "Invalid predecessor found.");

        // Instantly convert local vertex id path to global edge id path, meeting point -> target.
        while (b != localTarget) {
            unpackLowerTriangles(false, b, c, findEdge(cchPreprocessor->upwardsGraph.adjacencyIndices,
                                                       cchPreprocessor->upwardsGraph.head, b, c),
                                 onMissingEdgeFound);
            c = b;
            assert(b != INVALID_VALUE < VertexId > && "Invalid predecessor found.");
            b = biDirectionalDijkstra.backwardPredecessor[b];
        }
        unpackLowerTriangles(false, b, c, findEdge(cchPreprocessor->upwardsGraph.adjacencyIndices,
                                                   cchPreprocessor->upwardsGraph.head, b, c),
                             onMissingEdgeFound);
    }
    return edgePath;
}

template<typename WeightType>
std::vector<OptimizedKit::VertexId> OptimizedKit::CchQuery<WeightType>::getVertexPath() {
    assert(state == QueryState::FINISHED);
    vertexPath.clear();

    // Check success of Dijkstra.
    if (getQueryWeight() == INFINITY_WEIGHT < WeightType > ||
        biDirectionalDijkstra.meetingVertex == INVALID_VALUE < VertexId >) {
        return vertexPath;
    }

    auto onMissingVertexFound = [&](VertexId cchVertex, EdgeId cchEdge, bool forward) {
        (void) cchEdge;
        (void) forward;
        vertexPath.push_back(cchPreprocessor->order[cchVertex]);
    };

    // Path source -> meeting node.
    {
        std::vector<VertexId> localForwardPath;
        VertexId x = biDirectionalDijkstra.meetingVertex;
        while (x != localSource) {
            localForwardPath.push_back(x);
            x = biDirectionalDijkstra.forwardPredecessor[x];
            assert(x != INVALID_VALUE < VertexId > && "Invalid predecessor found.");
        }
        localForwardPath.push_back(localSource);

        // Convert local vertex id path to global vertex id path, source -> meeting point.
        for (unsigned i = localForwardPath.size() - 1; i != 0; --i) {
            unpackLowerTriangles(true, localForwardPath[i], localForwardPath[i - 1],
                                 findEdge(cchPreprocessor->upwardsGraph.adjacencyIndices,
                                          cchPreprocessor->upwardsGraph.head, localForwardPath[i],
                                          localForwardPath[i - 1]), onMissingVertexFound);
        }
    }

    // Path meeting node -> target.
    {
        VertexId c = biDirectionalDijkstra.meetingVertex;
        if (c == localTarget) {
            vertexPath.push_back(cchPreprocessor->order[c]);
            return vertexPath;
        }
        VertexId b = biDirectionalDijkstra.backwardPredecessor[c];
        assert(b != INVALID_VALUE < VertexId > && "Invalid predecessor found.");

        // Instantly convert local vertex id path to global vertex id path, meeting point -> target.
        while (b != localTarget) {
            unpackLowerTriangles(false, b, c, findEdge(cchPreprocessor->upwardsGraph.adjacencyIndices,
                                                       cchPreprocessor->upwardsGraph.head, b, c),
                                 onMissingVertexFound);
            c = b;
            b = biDirectionalDijkstra.backwardPredecessor[b];
            assert(b != INVALID_VALUE < VertexId > && "Invalid predecessor found.");
        }
        unpackLowerTriangles(false, b, c, findEdge(cchPreprocessor->upwardsGraph.adjacencyIndices,
                                                   cchPreprocessor->upwardsGraph.head, b, c),
                             onMissingVertexFound);
        vertexPath.push_back(cchPreprocessor->order[b]);
    }
    return vertexPath;
}

template<typename WeightType>
WeightType OptimizedKit::CchQuery<WeightType>::getQueryWeight() {
    assert(state == QueryState::FINISHED);
    if (biDirectionalDijkstra.meetingVertex == INVALID_VALUE < VertexId >)
        return INFINITY_WEIGHT<WeightType>;
    return biDirectionalDijkstra.shortestPathLength;
}

template<typename WeightType>
OptimizedKit::QueryState OptimizedKit::CchQuery<WeightType>::getState() {
    return state;
}
