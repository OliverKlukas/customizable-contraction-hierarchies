#include "utils/graph_helper.hpp"

OptimizedKit::EdgeId
OptimizedKit::findEdge(const std::vector<VertexId> &adjacencyList, const std::vector<VertexId> &head,
                       VertexId x, VertexId y) {
    assert(x < adjacencyList.size()-2);
    assert(y < adjacencyList.size()-1);
    auto begin = head.begin() + adjacencyList[x];
    auto end = head.begin() + adjacencyList[x+1];
    assert(std::is_sorted(begin, end));
    auto pos = std::lower_bound(begin, end, y);
    assert(pos != end && *pos == y);
    return pos - head.begin();
}

std::vector<OptimizedKit::VertexId> OptimizedKit::convertEdgePathToVertexPath(const std::vector<VertexId> &tail,
                                                                              const std::vector<VertexId> &head,
                                                                              std::vector<EdgeId> &edgePath) {
    assert(isVectorSorted(tail));
    std::vector<VertexId> vertexPath;
    vertexPath.resize(edgePath.size() + 1);
    for (auto arcId : edgePath)
        vertexPath.push_back(tail[arcId]);
    if (!vertexPath.empty())
        vertexPath.push_back(head[edgePath.back()]);
    return vertexPath;
}

std::vector<OptimizedKit::EdgeId> OptimizedKit::convertVertexPathToEdgePath(const std::vector<VertexId> &tail,
                                                                            const std::vector<VertexId> &head,
                                                                            std::vector<VertexId> &vertexPath) {
    assert(isVectorSorted(head));
    std::vector<EdgeId> arcPath;
    arcPath.resize(vertexPath.size() - 1);
    for (unsigned i = 0; i < vertexPath.size() - 1; ++i)
        arcPath.push_back(findEdge(tail, head, vertexPath[i], vertexPath[i + 1]));
    return arcPath;
}

