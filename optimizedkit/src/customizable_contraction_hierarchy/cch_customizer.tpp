#include <customizable_contraction_hierarchy/cch_customizer.hpp>

template <typename WeightType>
OptimizedKit::CchCustomizer<WeightType>::CchCustomizer(OptimizedKit::CchPreprocessor &preprocessor, const std::vector<WeightType> &weights, HeapType heapType_, bool debug_){
    assert(preprocessor.inputGraph.getEdgeCount() == weights.size());
    forwardWeights = std::vector<WeightType>(preprocessor.cchEdgeCount());
    backwardWeights = std::vector<WeightType>(preprocessor.cchEdgeCount());
    inputWeights = &weights[0];
    cchPreprocessor = &preprocessor;
    heapType = heapType_;
    state = CustomizerState::UNCUSTOMIZED;
    debug = debug_;
}

template <typename WeightType>
OptimizedKit::CchCustomizer<WeightType>::CchCustomizer(OptimizedKit::CchPreprocessor &preprocessor, const WeightType *weights, HeapType heapType_, bool debug_){
    forwardWeights = std::vector<WeightType>(preprocessor.cchEdgeCount());
    backwardWeights = std::vector<WeightType>(preprocessor.cchEdgeCount());
    inputWeights = weights;
    cchPreprocessor = &preprocessor;
    heapType = heapType_;
    state = CustomizerState::UNCUSTOMIZED;
    debug = debug_;
}

template<typename WeightType>
OptimizedKit::CchCustomizer<WeightType> &OptimizedKit::CchCustomizer<WeightType>::reset(OptimizedKit::CchPreprocessor &preprocessor,
                                                                                        const WeightType *weights) {
    if(preprocessor.cchEdgeCount() != forwardWeights.size()){
        forwardWeights = std::vector<WeightType>(preprocessor.cchEdgeCount());
        backwardWeights = std::vector<WeightType>(preprocessor.cchEdgeCount());
    }
    inputWeights = weights;
    cchPreprocessor = &preprocessor;
    state = CustomizerState::UNCUSTOMIZED;
    return *this;
}

template<typename WeightType>
OptimizedKit::CchCustomizer<WeightType> &OptimizedKit::CchCustomizer<WeightType>::reset(OptimizedKit::CchPreprocessor &preprocessor,
                                                                                        const std::vector<WeightType> &weights) {
    assert(preprocessor.inputGraph.getEdgeCount() == weights.size());
    reset(preprocessor, &weights[0]);
    return *this;
}

template<typename WeightType>
OptimizedKit::CchCustomizer<WeightType> &OptimizedKit::CchCustomizer<WeightType>::reset(const WeightType *weights) {
    inputWeights = weights;
    state = CustomizerState::UNCUSTOMIZED;
    return *this;
}

template<typename WeightType>
OptimizedKit::CchCustomizer<WeightType> &OptimizedKit::CchCustomizer<WeightType>::reset(const std::vector<WeightType> &weights) {
    assert(cchPreprocessor->inputGraph.getEdgeCount() == weights.size());
    reset(&weights[0]);
    return *this;
}

template<typename WeightType>
void OptimizedKit::CchCustomizer<WeightType>::extractEdgeWeight(EdgeId edge){
    // Check if edge is a shortcut edge.
    if(!cchPreprocessor->doesCchEdgeHaveInputEdge[edge]) {
        forwardWeights[edge] = INFINITY_WEIGHT<WeightType>;
        backwardWeights[edge] = INFINITY_WEIGHT<WeightType>;
        return;
    }

    // Extract weights based on existence as input edges.
    auto localId = cchPreprocessor->doesCchEdgeHaveInputEdgeMapper.toLocal(edge);
    auto forwardEdgeId = cchPreprocessor->forwardInputEdgeOfCchEdge[localId];
    forwardWeights[edge] = forwardEdgeId == INVALID_VALUE < EdgeId > ? INFINITY_WEIGHT < WeightType > : inputWeights[forwardEdgeId];
    auto backwardEdgeId = cchPreprocessor->backwardInputEdgeOfCchEdge[localId];
    backwardWeights[edge] = backwardEdgeId == INVALID_VALUE < EdgeId > ? INFINITY_WEIGHT < WeightType > : inputWeights[backwardEdgeId];

    // Check if extra input edges exist.
    if(!cchPreprocessor->doesCchEdgeHaveExtraInputEdge[edge])
        return;

    // Minimize edge distance based on all input edges.
    localId = cchPreprocessor->doesCchEdgeHaveExtraInputEdgeMapper.toLocal(edge);
    for(auto extraId = cchPreprocessor->extraForwardInputEdgeOfCchAdjacencyEdges[localId];
        extraId < cchPreprocessor->extraForwardInputEdgeOfCchAdjacencyEdges[localId + 1]; ++extraId)
        updateIfSmaller(forwardWeights[edge], inputWeights[cchPreprocessor->extraForwardInputEdgeOfCch[extraId]]);
    for(auto extraId = cchPreprocessor->extraBackwardInputEdgeOfCchAdjacencyEdges[localId];
        extraId < cchPreprocessor->extraBackwardInputEdgeOfCchAdjacencyEdges[localId + 1]; ++extraId)
        updateIfSmaller(backwardWeights[edge], inputWeights[cchPreprocessor->extraBackwardInputEdgeOfCch[extraId]]);
}

template<typename WeightType>
void OptimizedKit::CchCustomizer<WeightType>::extractRespectingMetric() {
    for (EdgeId edge = 0; edge < cchPreprocessor->cchEdgeCount(); ++edge)
        extractEdgeWeight(edge);
}

template<typename WeightType>
void OptimizedKit::CchCustomizer<WeightType>::relaxLowerTriangle(EdgeId ab,
                                                                 EdgeId ac,
                                                                 EdgeId bc,
                                                                 VertexId a,
                                                                 VertexId b,
                                                                 VertexId c) {
    updateIfSmaller(forwardWeights[bc], backwardWeights[ab] + forwardWeights[ac]);
    updateIfSmaller(backwardWeights[bc], forwardWeights[ab] + backwardWeights[ac]);
}

template<typename WeightType>
OptimizedKit::CchCustomizer<WeightType> &OptimizedKit::CchCustomizer<WeightType>::baseCustomization() {
    // Construct respecting metric based on weights.
    extractRespectingMetric();

    // Enumerate over all lower triangles of all edges in cch graph ordered increasingly by rank.
    std::vector<EdgeId> bcIds(cchPreprocessor->cchVertexCount());
    for(VertexId b = 0; b < cchPreprocessor->cchVertexCount(); ++b){
        // Save bc edge id at the index of c (head of bc) for all bc edges of b so that one only has to iterate through once.
        EdgeId bUpEnd = cchPreprocessor->upwardsGraph.adjacencyIndices[b + 1];
        for(EdgeId bc = cchPreprocessor->upwardsGraph.adjacencyIndices[b]; bc < bUpEnd; ++bc){
            bcIds[cchPreprocessor->upwardsGraph.head[bc]] = bc;
        }

        // Construct ab and ac edges and relax together with bc edge.
        for(EdgeId ba = cchPreprocessor->downwardsGraph.adjacencyIndices[b]; ba < cchPreprocessor->downwardsGraph.adjacencyIndices[b + 1]; ++ba){
            EdgeId ab = cchPreprocessor->downwardsToUpwardsGraph[ba];
            VertexId a = cchPreprocessor->downwardsGraph.head[ba];

            // Speed up by iterating from the highest rank downwards.
            assert(cchPreprocessor->upwardsGraph.adjacencyIndices[a] <= ab);
            for(EdgeId aUpReversed = cchPreprocessor->upwardsGraph.adjacencyIndices[a + 1]; aUpReversed > ab; --aUpReversed){
                EdgeId ac = aUpReversed - 1;
                VertexId c = cchPreprocessor->upwardsGraph.head[ac];
                if (c <= b) { break; }
                relaxLowerTriangle(ab, ac, bcIds[c], a, b, c);
            }
        }
    }
    state = CustomizerState::BASE_CUSTOMIZED;
    return *this;
}

template<typename WeightType>
OptimizedKit::CchCustomizer<WeightType> &OptimizedKit::CchCustomizer<WeightType>::perfectCustomization() {
    // Ensure that base customization has been performed.
    if(state == CustomizerState::UNCUSTOMIZED)
        baseCustomization();

    // Mark edges to be deleted during perfect witness search.
    Filter removeForwardEdgeFilter(cchPreprocessor->cchEdgeCount(), false);
    Filter removeBackwardEdgeFilter(cchPreprocessor->cchEdgeCount(), false);
    for(EdgeId edge = 0; edge < cchPreprocessor->cchEdgeCount(); ++edge){
        removeForwardEdgeFilter[edge] = forwardWeights[edge] == INFINITY_WEIGHT<WeightType>;
        removeBackwardEdgeFilter[edge] = backwardWeights[edge] == INFINITY_WEIGHT<WeightType>;
    }

    // Enumerate over all edges decreasing by rank, and relax the intermediate and upper triangles.
    for(EdgeId edge = cchPreprocessor->cchEdgeCount(); edge > 0; --edge){
        enumerateUpperTriangles(*cchPreprocessor, edge - 1, [&](EdgeId ab, EdgeId ac, EdgeId bc, VertexId a, VertexId b, VertexId c){
            // Upper triangles check.
            if(forwardWeights[ab] > forwardWeights[ac] + backwardWeights[bc]){
                forwardWeights[ab] = forwardWeights[ac] + backwardWeights[bc];
                removeForwardEdgeFilter[ab] = true;
            }
            if(backwardWeights[ab] > backwardWeights[ac] + forwardWeights[bc]){
                backwardWeights[ab] = backwardWeights[ac] + forwardWeights[bc];
                removeBackwardEdgeFilter[ab] = true;
            }

            // Intermediate triangles check.
            if(forwardWeights[ac] > forwardWeights[ab] + forwardWeights[bc]){
                forwardWeights[ac] = forwardWeights[ab] + forwardWeights[bc];
                removeForwardEdgeFilter[ac] = true;
            }
            if(backwardWeights[ac] > backwardWeights[ab] + backwardWeights[bc]){
                backwardWeights[ac] = backwardWeights[ab] + backwardWeights[bc];
                removeBackwardEdgeFilter[ac] = true;
            }
        });
    }

    // If debug mode then calculate the number of changed weights by the perfect customization.
    if(debug) {
        numChangedWeights = 0;
        for (EdgeId edge = 0; edge < cchPreprocessor->cchEdgeCount(); ++edge) {
            if (!cchPreprocessor->doesCchEdgeHaveInputEdge[edge])
                continue;
            if (removeBackwardEdgeFilter[edge] != removeForwardEdgeFilter[edge])
                ++numChangedWeights;
        }
        std::cout << "INFO: Number of changed weights by perfect customization: " << numChangedWeights << std::endl;
    }

    // Delete edges during perfect witness search that are not part of the shortest paths.
    Filter removeEdgeFilter(cchPreprocessor->cchEdgeCount(), false);
    assert(removeBackwardEdgeFilter.size() == removeForwardEdgeFilter.size());
    assert(removeBackwardEdgeFilter.size() == cchPreprocessor->cchEdgeCount());
    for(unsigned i = 0; i < cchPreprocessor->cchEdgeCount(); i++){
        removeEdgeFilter[i] = removeForwardEdgeFilter[i] && removeBackwardEdgeFilter[i];
    }
    removeElementsByFilterInplace(forwardWeights, removeEdgeFilter);
    removeElementsByFilterInplace(backwardWeights, removeEdgeFilter);
    this->cchPreprocessor->removeEdges(removeEdgeFilter);

    state = CustomizerState::PERFECT_CUSTOMIZED;
    return *this;
}


template<typename WeightType>
OptimizedKit::CchCustomizer<WeightType> &OptimizedKit::CchCustomizer<WeightType>::update(const std::vector<unsigned int> &updateIds) {
    assert(state != CustomizerState::UNCUSTOMIZED && "Customizer must be customized before updating.");

    // Extract all desired updates.
    OptimizedKit::AbstractHeap<unsigned, unsigned>* q;
    switch (heapType) {
        case HeapType::BINARY:
            q = new BinaryMinHeap<unsigned, unsigned>();
            break;
        case HeapType::PAIRING:
            q = new PairingMinHeap<unsigned, unsigned>();
            break;
        default:
            throw std::runtime_error("Heap type not supported.");
    }

    for(auto update : updateIds){
        assert(update < cchPreprocessor->inputEdgeToCchEdge.size() && "Update id out of bounds.");
        auto edge = cchPreprocessor->inputEdgeToCchEdge[update];
        if(edge != INVALID_VALUE < EdgeId >)
            q->insertOrUpdate(edge);
    }

    // Re-customize edges in increasing order of id.
    while(!q->isEmpty()){
        EdgeId uv = q->deleteMin();

        // Save old weights before reset to determine if full triangle enumeration is necessary.
        auto prevForwardWeight = forwardWeights[uv];
        auto prevBackwardWeight = backwardWeights[uv];
        forwardWeights[uv] = INFINITY_WEIGHT<WeightType>;
        backwardWeights[uv] = INFINITY_WEIGHT<WeightType>;

        // Re-compute basic weights
        extractEdgeWeight(uv);
        enumerateLowerTriangles(*cchPreprocessor, uv, [&](EdgeId ab, EdgeId ac, EdgeId bc, VertexId a, VertexId b, VertexId c){
            relaxLowerTriangle(ab, ac, bc, a, b, c);
        });

        // Check if other edges might be affected.
        if(forwardWeights[uv] == prevForwardWeight && backwardWeights[uv] == prevBackwardWeight)
            continue;

        // Add other edges to partial update if they were affected by the partial update.
        enumerateIntermediateTriangles(*cchPreprocessor,uv, [&](EdgeId ab, EdgeId ac, EdgeId bc, VertexId a, VertexId b, VertexId c){
            if(
                    backwardWeights[ab] + prevForwardWeight == forwardWeights[bc] ||
                    prevBackwardWeight + forwardWeights[ab] == backwardWeights[bc] ||
                    backwardWeights[ab] + forwardWeights[uv] < forwardWeights[bc] ||
                    backwardWeights[uv] + forwardWeights[ab] < backwardWeights[bc]
                    ){
                q->insertOrUpdate(bc);
            }
        });
        enumerateUpperTriangles(*cchPreprocessor,uv, [&](EdgeId ab, EdgeId ac, EdgeId bc, VertexId a, VertexId b, VertexId c){
            if(
                    prevBackwardWeight + forwardWeights[ac] == forwardWeights[bc] ||
                    backwardWeights[ac] + prevForwardWeight == backwardWeights[bc] ||
                    backwardWeights[uv] + forwardWeights[ac] < forwardWeights[bc] ||
                    backwardWeights[ac] + forwardWeights[uv] < backwardWeights[bc]
                    ){
                q->insertOrUpdate(bc);
            }
        });
    }

    delete q;
    return *this;
}
