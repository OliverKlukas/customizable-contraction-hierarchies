#include <path_finding_algorithms/dijkstra.hpp>

template<typename WeightType>
OptimizedKit::Dijkstra<WeightType> &OptimizedKit::Dijkstra<WeightType>::initialize(VertexId sourceId, bool debug) {
    // Initialize all values to default for source query.
    source = sourceId;
    distances.clear();
    distances.resize(vertexCount, INFINITY_WEIGHT<WeightType>);
    distances[source] = 0;
    predecessors.clear();
    predecessors.resize(vertexCount, INVALID_VALUE<VertexId>);
    queue.clear();
    for (VertexId i = 0; i < vertexCount; ++i)
        queue.insertOrUpdate(distances[i], i);

    // Compute all distances from source in graph.
    while(!queue.isEmpty()){
        auto u = queue.deleteMin().id;
        for (auto neighbour = cchGraph.upwardsGraph->adjacencyIndices->at(u);
             neighbour < cchGraph.upwardsGraph->adjacencyIndices->at(u + 1); ++neighbour) {
            auto v = cchGraph.upwardsGraph->head[neighbour];
            auto weight = std::min(cchGraph.forwardWeights->at(neighbour), cchGraph.backwardWeights->at(neighbour)); // TODO: this is were this falls apart as one has to do a dijkstra from source to meeting point with forward weights and one dijkstra from target to meeting point and then you basically have a bi-directional dijkstra again.
            if(distances[v] > distances[u] + weight){
                distances[v] = distances[u] + weight;
                predecessors[v] = u;
                queue.decreaseKey(v, distances[v]);
            }
        }
    }
    return *this;
}

template<typename WeightType>
WeightType OptimizedKit::Dijkstra<WeightType>::getQueryWeight(VertexId target, bool debug) {
    assert(source != INVALID_VALUE<VertexId> && "source vertex must be initialized");
    assert(target < vertexCount && "target vertex must be less than vertexCount");
    return distances[target];
}

template<typename WeightType>
std::vector<OptimizedKit::EdgeId> OptimizedKit::Dijkstra<WeightType>::getEdgePath(VertexId target, bool debug) {
    assert(source != INVALID_VALUE<VertexId> && "source vertex must be initialized");
    assert(target < vertexCount && "target vertex must be less than vertexCount");
    std::vector<EdgeId> arcPath;
    auto vertexPath = getVertexPath(source, debug);
    if(vertexPath.size() < 2)
        return arcPath;
    for(auto i = 1; i < vertexPath.size(); ++i){
        auto u = vertexPath[i-1];
        auto v = vertexPath[i];
        for(auto j = 0; j < cchGraph.upwardsGraph->tail.size(); j++){
            if(cchGraph.upwardsGraph->tail[j] == u && cchGraph.upwardsGraph->head[j] == v){
                arcPath.push_back(j);
                break;
            }
        }
    }
    return arcPath;
}

template<typename WeightType>
std::vector<OptimizedKit::VertexId> OptimizedKit::Dijkstra<WeightType>::getVertexPath(VertexId target, bool debug) {
    assert(source != INVALID_VALUE<VertexId> && "source vertex must be initialized");
    assert(target < vertexCount && "target vertex must be less than vertexCount");
    std::vector<VertexId> vertexPath;
    auto u = target;
    if(predecessors[u] != INVALID_VALUE<VertexId> || u == source){
        while(u != INVALID_VALUE<VertexId>){
            vertexPath.push_back(u);
            u = predecessors[u];
        }
    }
    std::reverse(vertexPath.begin(), vertexPath.end());
    return vertexPath;
}

template<typename WeightType>
void OptimizedKit::Dijkstra<WeightType>::ensureCorrectInitialization() {
    assert(source != INVALID_VALUE<VertexId> && "source vertex must be initialized");
    assert(source < vertexCount && "source vertex must be less than vertexCount");
    assert(distances.size() == vertexCount && "distances vector must have the same size as vertexCount");
    assert(predecessors.size() == vertexCount && "predecessors vector must have the same size as vertexCount");
    assert(queue.size() == vertexCount && "queue must have the same size as vertexCount");
    assert(queue.peek().id == source && "queue must have the source vertex as the first element");
}