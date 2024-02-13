#include <customizable_contraction_hierarchy/cch_preprocessor.hpp>

OptimizedKit::CchPreprocessor::CchPreprocessor(OptimizedKit::Order inputOrder, OptimizedKit::Graph graph) {
    // Initialize preprocessing phase variables.
    order = std::move(inputOrder);
    inputGraph = std::move(graph);
    rank = invertPermutation(order);

    // Preprocessing phase steps.
    applyOrder();
    sortGraph();
    buildUpwardsGraph();
    buildInputToCchMapping();
    buildDownwardsGraph();
    buildCchToInputMapping();
}

void OptimizedKit::CchPreprocessor::applyOrder() {
    inputGraph.tail = applyPermutationToElementsOf(rank, inputGraph.tail);
    inputGraph.head = applyPermutationToElementsOf(rank, inputGraph.head);
}

void OptimizedKit::CchPreprocessor::sortGraph() {
    inputEdgeIds = computeSortPermutationFirstByTailThenByHeadAndApplySortToTail(inputGraph.tail, inputGraph.head);
    inputGraph.head = applyPermutation(inputEdgeIds, inputGraph.head);
}

void OptimizedKit::CchPreprocessor::buildUpwardsGraph() {
    // Create a symmetric graph, essentially directing all edges in both directions.
    std::vector<unsigned> symmetricTail(inputGraph.getEdgeCount() * 2);
    std::vector<unsigned> symmetricHead(inputGraph.getEdgeCount() * 2);
    std::copy(inputGraph.tail.begin(), inputGraph.tail.end(), symmetricTail.begin());
    std::copy(inputGraph.head.begin(), inputGraph.head.end(), symmetricHead.begin());
    std::copy(inputGraph.tail.begin(), inputGraph.tail.end(), symmetricHead.begin() + inputGraph.getEdgeCount());
    std::copy(inputGraph.head.begin(), inputGraph.head.end(), symmetricTail.begin() + inputGraph.getEdgeCount());

    // Remove self-loops and duplicate edges, neighbor comparison sufficient as edge lists are sorted.
    Filter filter(inputGraph.getEdgeCount() * 2);
    if (inputGraph.getEdgeCount() != 0)
        filter[0] = symmetricTail[0] == symmetricHead[0];
    for (unsigned i = 1; i < inputGraph.getEdgeCount() * 2; ++i)
        filter[i] = symmetricTail[i] == symmetricHead[i] ||
                (symmetricHead[i] == symmetricHead[i - 1] && symmetricTail[i] == symmetricTail[i - 1]);
    removeElementsByFilterInplace(symmetricTail, filter);
    removeElementsByFilterInplace(symmetricHead, filter);

    // Create an adjacency list of the symmetric graph.
    std::vector<std::vector<VertexId>> upwardsAdjacencyList(inputVertexCount());
    for (unsigned i = 0; i < symmetricTail.size(); ++i) {
        if (symmetricTail[i] < symmetricHead[i])
            upwardsAdjacencyList[symmetricTail[i]].push_back(symmetricHead[i]);
    }

    // Sort the adjacency list and remove adjacent duplicates.
    for (auto &list: upwardsAdjacencyList) {
        std::sort(list.begin(), list.end());
        list.erase(std::unique(list.begin(), list.end()), list.end());
    }

    // Temporarily contract vertices and add shortcut where applicable.
    for (VertexId vertex = 0; vertex < inputVertexCount(); ++vertex) {
        if (upwardsAdjacencyList[vertex].empty())
            continue;
        const VertexId lowestNeighbour = upwardsAdjacencyList[vertex][0];
        std::vector<VertexId> mergedNeighbours(
                upwardsAdjacencyList[vertex].size() + upwardsAdjacencyList[lowestNeighbour].size() - 1);
        std::merge(++upwardsAdjacencyList[vertex].begin(), upwardsAdjacencyList[vertex].end(),
                   upwardsAdjacencyList[lowestNeighbour].begin(), upwardsAdjacencyList[lowestNeighbour].end(),
                   mergedNeighbours.begin());
        mergedNeighbours.erase(std::unique(mergedNeighbours.begin(), mergedNeighbours.end()), mergedNeighbours.end());
        upwardsAdjacencyList[lowestNeighbour] = std::move(mergedNeighbours);
        for (auto neighbour: upwardsAdjacencyList[vertex])
            upwardsGraph.addEdge(vertex, neighbour);
    }
    upwardsGraph.head.shrink_to_fit();
    upwardsGraph.tail.shrink_to_fit();
    upwardsGraph.vertexCount = cchVertexCount();
    auto p = computeInverseSortPermutationFirstByTailThenByHeadAndApplySortToTail(upwardsGraph.tail, upwardsGraph.head);
    upwardsGraph.head = applyInversePermutation(p, upwardsGraph.head);
    upwardsGraph.createAdjacencyIndices();
}

void OptimizedKit::CchPreprocessor::buildInputToCchMapping() {
    isInputEdgeUpwards.resize(inputGraph.getEdgeCount(), false);
    if (upwardsGraph.getEdgeCount() == 0) {
        inputEdgeToCchEdge.resize(inputGraph.getEdgeCount(), OptimizedKit::INVALID_VALUE<unsigned>);
        return;
    }
    inputEdgeToCchEdge.resize(inputGraph.getEdgeCount());

    // Map input edges to cch edges and identify loops in the input graph.
    EdgeId cchUpEdge = 0;
    for (EdgeId inputEdge = 0; inputEdge < inputGraph.getEdgeCount(); ++inputEdge) {
        // Only consider upward edges.
        if (inputGraph.tail[inputEdge] < inputGraph.head[inputEdge]) {
            // Identify a match between input and cch graph edges.
            while (inputGraph.head[inputEdge] != upwardsGraph.head[cchUpEdge] ||
                   inputGraph.tail[inputEdge] != upwardsGraph.tail[cchUpEdge]) {
                assert(cchUpEdge < upwardsGraph.getEdgeCount());
                ++cchUpEdge;
            }
            assert(inputGraph.head[inputEdge] == upwardsGraph.head[cchUpEdge]);
            assert(inputGraph.tail[inputEdge] == upwardsGraph.tail[cchUpEdge]);

            // Make sure to utilize the original ids.
            inputEdgeToCchEdge[inputEdgeIds[inputEdge]] = cchUpEdge;
            isInputEdgeUpwards[inputEdgeIds[inputEdge]] = true;
        }
            // Mark loops as invalid.
        else if (inputGraph.head[inputEdge] == inputGraph.tail[inputEdge]) {
            inputEdgeToCchEdge[inputEdgeIds[inputEdge]] = OptimizedKit::INVALID_VALUE<EdgeId>;
        }
    }

    // Swap and re-sort the input tail and head.
    std::swap(inputGraph.tail, inputGraph.head);
    auto p = computeInverseSortPermutationFirstByTailThenByHeadAndApplySortToTail(inputGraph.tail, inputGraph.head);
    inputGraph.head = applyInversePermutation(p, inputGraph.head);
    inputEdgeIds = applyInversePermutation(p, inputEdgeIds);

    // Re-do the mapping but this time for downwards edges.
    EdgeId cchDownEdge = 0;
    for (EdgeId inputEdge = 0; inputEdge < inputGraph.getEdgeCount(); ++inputEdge) {
        // Only consider upwards edges.
        if (inputGraph.tail[inputEdge] < inputGraph.head[inputEdge]) {
            while (inputGraph.head[inputEdge] != upwardsGraph.head[cchDownEdge] ||
                   inputGraph.tail[inputEdge] != upwardsGraph.tail[cchDownEdge]) {
                assert(cchDownEdge < upwardsGraph.getEdgeCount());
                ++cchDownEdge;
            }
            inputEdgeToCchEdge[inputEdgeIds[inputEdge]] = cchDownEdge;
        }
    }

    // Also compute the reverse mapping.
    cchEdgeToInputEdge.resize(upwardsGraph.getEdgeCount());
    for (EdgeId inputEdge = 0; inputEdge < inputGraph.getEdgeCount(); ++inputEdge) {
        if (inputEdgeToCchEdge[inputEdge] != OptimizedKit::INVALID_VALUE<EdgeId>) {
            cchEdgeToInputEdge[inputEdgeToCchEdge[inputEdge]] = inputEdge;
        }
    }
}

void OptimizedKit::CchPreprocessor::buildDownwardsGraph() {
    downwardsGraph.head = upwardsGraph.tail;
    downwardsGraph.tail = upwardsGraph.head;
    downwardsGraph.vertexCount = cchVertexCount();
    downwardsToUpwardsGraph = computeSortPermutationFirstByTailThenByHeadAndApplySortToTail(downwardsGraph.tail,
                                                                                            downwardsGraph.head);
    downwardsGraph.head = applyPermutation(downwardsToUpwardsGraph, std::move(downwardsGraph.head));
    downwardsGraph.createAdjacencyIndices();
}

void OptimizedKit::CchPreprocessor::buildCchToInputMapping() {
    doesCchEdgeHaveInputEdge.resize(upwardsGraph.getEdgeCount(), false);

    // Persist state mappings from input edge to cch edge.
    for (EdgeId inputEdge = 0; inputEdge < inputGraph.getEdgeCount(); ++inputEdge) {
        if (inputEdgeToCchEdge[inputEdge] != OptimizedKit::INVALID_VALUE<EdgeId>) {
            doesCchEdgeHaveInputEdge[inputEdgeToCchEdge[inputEdge]] = true;
        }
    }

    // Initialize forward and backward input edge vectors.
    doesCchEdgeHaveInputEdgeMapper = IdMapper(doesCchEdgeHaveInputEdge);
    forwardInputEdgeOfCchEdge.resize(doesCchEdgeHaveInputEdgeMapper.getLocalIdCount());
    backwardInputEdgeOfCchEdge.resize(doesCchEdgeHaveInputEdgeMapper.getLocalIdCount());
    std::fill(forwardInputEdgeOfCchEdge.begin(), forwardInputEdgeOfCchEdge.end(), OptimizedKit::INVALID_VALUE<EdgeId>);
    std::fill(backwardInputEdgeOfCchEdge.begin(), backwardInputEdgeOfCchEdge.end(), OptimizedKit::INVALID_VALUE<EdgeId>);
    doesCchEdgeHaveExtraInputEdge.resize(upwardsGraph.getEdgeCount(), false);

    // Identify edges that were added extra to the cch graph and map upwards/downwards to forward/backward edges.
    for (EdgeId inputEdge = 0; inputEdge < inputGraph.getEdgeCount(); ++inputEdge) {
        auto cchEdge = inputEdgeToCchEdge[inputEdge];
        if (cchEdge != OptimizedKit::INVALID_VALUE<EdgeId>) {
            auto id = doesCchEdgeHaveInputEdgeMapper.toLocal(cchEdge);
            if (isInputEdgeUpwards[inputEdge]) {
                if (forwardInputEdgeOfCchEdge[id] == OptimizedKit::INVALID_VALUE<EdgeId>) {
                    forwardInputEdgeOfCchEdge[id] = inputEdge;
                } else {
                    doesCchEdgeHaveExtraInputEdge[cchEdge] = true;
                    extraForwardInputEdgeOfCchAdjacencyEdges.push_back(cchEdge);
                    extraForwardInputEdgeOfCch.push_back(inputEdge);
                }
            } else {
                if (backwardInputEdgeOfCchEdge[id] == OptimizedKit::INVALID_VALUE<EdgeId>) {
                    backwardInputEdgeOfCchEdge[id] = inputEdge;
                } else {
                    doesCchEdgeHaveExtraInputEdge[cchEdge] = true;
                    extraBackwardInputEdgeOfCchAdjacencyEdges.push_back(cchEdge);
                    extraBackwardInputEdgeOfCch.push_back(inputEdge);
                }
            }
        }
    }

    // Map extra input edges to local ids.
    doesCchEdgeHaveExtraInputEdgeMapper = IdMapper(doesCchEdgeHaveExtraInputEdge);
    for (auto &x: extraForwardInputEdgeOfCchAdjacencyEdges)
        x = doesCchEdgeHaveExtraInputEdgeMapper.toLocal(x);
    for (auto &x: extraBackwardInputEdgeOfCchAdjacencyEdges)
        x = doesCchEdgeHaveExtraInputEdgeMapper.toLocal(x);

    // Sort extra input edges by cch edge id.
    {
        auto p = computeInverseStableSortPermutation(extraForwardInputEdgeOfCchAdjacencyEdges);
        extraForwardInputEdgeOfCchAdjacencyEdges =
                OptimizedKit::constructAdjacencyIndices(
                        applyInversePermutation(p, std::move(extraForwardInputEdgeOfCchAdjacencyEdges)),
                        doesCchEdgeHaveExtraInputEdgeMapper.getLocalIdCount());
        extraForwardInputEdgeOfCch = applyInversePermutation(p, std::move(extraForwardInputEdgeOfCch));
    }
    {
        auto p = computeInverseStableSortPermutation(extraBackwardInputEdgeOfCchAdjacencyEdges);
        extraBackwardInputEdgeOfCchAdjacencyEdges =
                OptimizedKit::constructAdjacencyIndices(
                        applyInversePermutation(p, std::move(extraBackwardInputEdgeOfCchAdjacencyEdges)),
                        doesCchEdgeHaveExtraInputEdgeMapper.getLocalIdCount());
        extraBackwardInputEdgeOfCch = applyInversePermutation(p, std::move(extraBackwardInputEdgeOfCch));
    }
}

void OptimizedKit::CchPreprocessor::removeEdges(const OptimizedKit::Filter &removeEdgeFilter) {
    assert(removeEdgeFilter.size() == cchEdgeCount());

    // Remove edges where applicable.
    upwardsGraph.removeEdges(removeEdgeFilter);
    downwardsGraph.removeEdges(removeEdgeFilter);

    // TODO: the error must be in these or a missing one (extra)
    removeElementsByFilterInplace(cchEdgeToInputEdge, removeEdgeFilter);
    removeElementsByFilterInplace(doesCchEdgeHaveInputEdge, removeEdgeFilter);
    doesCchEdgeHaveInputEdgeMapper.remove(removeEdgeFilter);
    removeElementsByFilterInplace(doesCchEdgeHaveExtraInputEdge, removeEdgeFilter);
    doesCchEdgeHaveExtraInputEdgeMapper.remove(removeEdgeFilter);

    // Alter edges ids of cch edges where necessary.
    adjustElementsToRemoveFilterInPlace(inputEdgeToCchEdge, removeEdgeFilter);
}
