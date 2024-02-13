#include <path_finding_algorithms/bi_directional_dijkstra.hpp>

template<typename WeightType>
void
OptimizedKit::BiDirectionalDijkstra<WeightType>::initialize() {
    assert(source < vertexCount && "Source is not set");
    assert(target < vertexCount && "Target is not set");

    meetingVertex = INVALID_VALUE<VertexId>;
    shortestPathLength = INFINITY_WEIGHT<WeightType>;

    forwardDistance.clear();
    forwardDistance.resize(vertexCount, INFINITY_WEIGHT<WeightType>);
    forwardDistance[source] = 0;
    backwardDistance.clear();
    backwardDistance.resize(vertexCount, INFINITY_WEIGHT<WeightType>);
    backwardDistance[target] = 0;

    forwardQueue->clear();
    forwardQueue->insertOrUpdate(0, source);
    backwardQueue->clear();
    backwardQueue->insertOrUpdate(0, target);

    forwardPredecessor.clear();
    forwardPredecessor.resize(vertexCount, INVALID_VALUE<VertexId>);
    backwardPredecessor.clear();
    backwardPredecessor.resize(vertexCount, INVALID_VALUE<VertexId>);

    forwardSettled.clear();
    forwardSettled.resize(vertexCount, false);
    backwardSettled.clear();
    backwardSettled.resize(vertexCount, false);
}

template<typename WeightType>
OptimizedKit::BiDirectionalDijkstra<WeightType> &OptimizedKit::BiDirectionalDijkstra<WeightType>::run(VertexId sourceId, VertexId targetId, bool debug) {
    source = sourceId;
    target = targetId;
    initialize();
    bool forwardSearchActive = true;
    bool backwardSearchActive = true;

    if(debug){
        std::cout << "Running search space analysis for source " << source << " and target " << target << std::endl;
        numEdgesExplored = 0;
        numVerticesExplored = 0;
    }

    while (forwardSearchActive || backwardSearchActive) {
        // Forward search.
        if(forwardSearchActive){
            auto u = forwardQueue->deleteMin();
            forwardSettled[u] = true;
            if(debug)
                numVerticesExplored++;

            // Check if forward search has reached backward search with a shorter path.
            if (backwardSettled[u] && shortestPathLength > forwardDistance[u] + backwardDistance[u]) {
                shortestPathLength = forwardDistance[u] + backwardDistance[u];
                meetingVertex = u;
                if (debug)
                    std::cout << "New shorter path in forward search between source " << source << " via meeting point "
                              << meetingVertex << " and target " << target << " with length: " << shortestPathLength
                              << std::endl;
            }

            // Relax all outgoing edges of (u,x) with weight in forward search.
            for (auto forwardArc = cchGraph.upwardsGraph->adjacencyIndices[u];
                 forwardArc < cchGraph.upwardsGraph->adjacencyIndices[u+1]; ++forwardArc) {
                auto x = cchGraph.upwardsGraph->head[forwardArc];

                if (forwardSettled[x])
                    continue;

                if(debug)
                    numEdgesExplored++;

                auto weight = cchGraph.forwardWeights->at(forwardArc);
                if (forwardDistance[x] > forwardDistance[u] + weight) {
                    forwardDistance[x] = forwardDistance[u] + weight;
                    forwardPredecessor[x] = u;
                    forwardQueue->insertOrUpdate(forwardDistance[x], x);
                }
            }
        }

        // Backward search.
        if(backwardSearchActive){
            auto v = backwardQueue->deleteMin();
            backwardSettled[v] = true;

            if(debug)
                numVerticesExplored++;

            // Check if backward search has reached forward search with a shorter path.
            if (forwardSettled[v] && shortestPathLength > forwardDistance[v] + backwardDistance[v]) {
                shortestPathLength = forwardDistance[v] + backwardDistance[v];
                meetingVertex = v;
                if (debug)
                    std::cout << "New shorter path in backward search between source " << source << " via meeting point "
                              << meetingVertex << " and target " << target << " with length: " << shortestPathLength
                              << std::endl;
            }

            // Relax all outgoing edges of (v,y) with weight in backward search.
            for (auto backwardArc = cchGraph.upwardsGraph->adjacencyIndices[v];
                 backwardArc < cchGraph.upwardsGraph->adjacencyIndices[v+1]; ++backwardArc) {
                auto y = cchGraph.upwardsGraph->head[backwardArc];

                if (backwardSettled[y])
                    continue;

                if(debug)
                    numEdgesExplored++;

                auto weight = cchGraph.backwardWeights->at(backwardArc);
                if (backwardDistance[y] > backwardDistance[v] + weight) {
                    backwardDistance[y] = backwardDistance[v] + weight;
                    backwardPredecessor[y] = v;
                    backwardQueue->insertOrUpdate(backwardDistance[y], y);
                }
            }
        }

        // Bi-directional termination criteria.
        forwardSearchActive = !forwardQueue->isEmpty() && forwardQueue->peek() < shortestPathLength;
        backwardSearchActive = !backwardQueue->isEmpty() && backwardQueue->peek() < shortestPathLength;
    }

    if(debug){
        std::cout << "Search space analysis for source " << source << " and target " << target << std::endl;
        std::cout << "Number of vertices explored: " << numVerticesExplored << std::endl;
        std::cout << "Number of edges explored: " << numEdgesExplored << std::endl;
    }
    return *this;
}

