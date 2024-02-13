#include <queue>
#include <stack>
#include <graph_order_algorithms/dinic_min_cut.hpp>
/* TODO: refactor to not rely on directed graph
template<typename WeightType>
bool OptimizedKit::DinicMinCut<WeightType>::hasNeighboursNotInSet(DinicMinCut::SourceSinkVertices &set,
                                                                  OldDirectedGraph <WeightType> &graph,
                                                                  OptimizedKit::VertexId vertexId) {
    const auto is_not_contained = [&set](const typename OptimizedKit::OldDirectedGraph<WeightType>::Edge &edge) {
        return set.find(edge.destination) == set.end();
    };
    return (graph.endOutgoingEdges(vertexId) !=
            std::find_if(graph.beginOutgoingEdges(vertexId), graph.endOutgoingEdges(vertexId), is_not_contained)) ||
           (graph.endIncomingEdges(vertexId) !=
            std::find_if(graph.beginIncomingEdges(vertexId), graph.endIncomingEdges(vertexId), is_not_contained));
}

template<typename WeightType>
OptimizedKit::DinicMinCut<WeightType>::MinCut
OptimizedKit::DinicMinCut<WeightType>::run(OldDirectedGraph <WeightType> &graph,
                                           SourceSinkVertices &sources,
                                           SourceSinkVertices &sinks) {
    // Only consider the border of the source and sink vertices to save initialization costs.
    std::vector<VertexId> border_source_vertices;
    border_source_vertices.reserve(std::size_t(0.01 * static_cast<double>(sources.size())));
    std::copy_if(sources.begin(), sources.end(), std::back_inserter(border_source_vertices),
                 [&](const auto &vertex) {
                     return hasNeighboursNotInSet(sources, graph, vertex);
                 });
    std::vector<VertexId> border_sink_vertices;
    border_sink_vertices.reserve(std::size_t(0.01 * static_cast<double>(sinks.size())));
    std::copy_if(sinks.begin(), sinks.end(), std::back_inserter(border_sink_vertices),
                 [&](const auto &vertex) {
                     return hasNeighboursNotInSet(sinks, graph, vertex);
                 });

    // As the graph is undirected and unweighted flow will be considered binary, the residual graph can therefore only
    // have one directional edges.
    FlowEdges flow(graph.getVertexCount());
    std::size_t flow_value = 0;
    do {
        // Calculate the levels of the vertices.
        auto levels = ComputeLevelGraph(graph, border_source_vertices, sources, sinks, flow);

        // Check if the sink can be reached from the source, it's enough to check the border.
        const auto separated = std::find_if(border_sink_vertices.begin(),
                                            border_sink_vertices.end(),
                                            [&levels, &graph](const auto vertex) {
                                                return levels[vertex] != INVALID_VALUE<Level>;
                                            }) == border_sink_vertices.end();

        if (!separated) {
            flow_value += BlockingFlow(flow, levels, graph, sources, border_sink_vertices);
        } else {
            // Mark levels for all sources to not confuse make-cut (due to the border vertices heuristic).
            for (auto s: sources)
                levels[s] = 0;
            auto cut = MakeCut(graph, levels, flow_value);
            return cut;
        }
    } while (true);

}

template<typename WeightType>
OptimizedKit::DinicMinCut<WeightType>::MinCut
OptimizedKit::DinicMinCut<WeightType>::MakeCut(OldDirectedGraph <WeightType> &graph, DinicMinCut::LevelGraph &levels,
                                               std::size_t flow_value) {
    const auto is_valid_level = [](const Level level) { return level != INVALID_VALUE<Level>; };

    std::vector<bool> result(graph.getVertexCount());

    assert(graph.getVertexCount() == levels.size());
    std::size_t source_side_count = std::count_if(levels.begin(), levels.end(), is_valid_level);
    std::transform(levels.begin(), levels.end(), result.begin(), is_valid_level);

    return {source_side_count, flow_value, std::move(result)};
}

template<typename WeightType>
std::vector<OptimizedKit::VertexId>
OptimizedKit::DinicMinCut<WeightType>::GetAugmentingPath(DinicMinCut::LevelGraph &levels, VertexId vertexId,
                                                         OldDirectedGraph <WeightType> &graph,
                                                         DinicMinCut::FlowEdges &flow,
                                                         DinicMinCut::SourceSinkVertices &source_vertices) {
    std::vector<VertexId> path;
    assert(source_vertices.find(vertexId) == source_vertices.end());

    // Keeps the local state of the DFS in forms of the iterators
    struct DFSState {
        std::vector<typename OldDirectedGraph<WeightType>::Edge>::const_iterator arcIterator;
        std::vector<typename OldDirectedGraph<WeightType>::Edge>::const_iterator endIterator;
    };

    std::stack<DFSState> dfs_stack;
    auto arcs = graph.getArcs(vertexId);
    DFSState initial_state = {arcs.begin(), arcs.end()};
    dfs_stack.push(std::move(initial_state));
    path.push_back(vertexId);

    while (!dfs_stack.empty()) {
        // the dfs_stack and the path have to be kept in sync
        assert(dfs_stack.size() == path.size());

        while (dfs_stack.top().arcIterator != dfs_stack.top().endIterator) {
            const auto destination = dfs_stack.top().arcIterator->destination;

            // look at every edge only once, so advance the state of the current node (last in
            // path)
            dfs_stack.top().arcIterator++;

            // check if the edge is state
            const auto has_capacity = flow[destination].count(path.back()) == 0;
            const auto descends_level_graph = levels[destination] + 1 == levels[path.back()];

            if (has_capacity && descends_level_graph) {
                // recurse
                path.push_back(destination);

                // termination
                if (source_vertices.find(destination) != source_vertices.end()) {
                    std::reverse(path.begin(), path.end());
                    return path;
                }

                // start next iteration
                arcs = graph.getArcs(destination);
                dfs_stack.push({arcs.begin(), arcs.end()});
            }
        }

        // backtrack - mark that there is no way to the target
        levels[path.back()] = -1;
        path.pop_back();
        dfs_stack.pop();
    }
    assert(path.empty());
    return path;
}

template<typename WeightType>
std::size_t
OptimizedKit::DinicMinCut<WeightType>::BlockingFlow(DinicMinCut::FlowEdges &flow, DinicMinCut::LevelGraph &levels,
                                                    OldDirectedGraph <WeightType> &graph,
                                                    DinicMinCut::SourceSinkVertices &source_vertices,
                                                    std::vector<VertexId> &border_sink_vertices) {
    // Equalling the number of augmenting paths, which in sum equals the number of unique border edges in the undirected graph.
    std::size_t flow_increase = 0;

    // Augments the flow along a path in the level graph.
    const auto augment_flow = [&flow, &graph](const std::vector<VertexId> &path) {

        // Add/Remove flow edges from the current residual graph.
        const auto augment_one = [&flow, &graph](const VertexId from, const VertexId to) {
            // Check if there is flow in the opposite direction.
            auto existing_edge = flow[to].find(from);
            if (existing_edge != flow[to].end())
                flow[to].erase(existing_edge); // Remove flow from reverse edges first.
            else
                flow[from].insert(to); // Only add flow if no opposite flow exists.

            // Do augmentation on all pairs, never stop early.
            return false;
        };

        // Augment all adjacent edges.
        std::adjacent_find(path.begin(), path.end(), augment_one);
    };

    const auto augment_all_paths = [&](const VertexId sink_node_id) {
        // Only augment sinks
        if (levels[sink_node_id] == INVALID_VALUE < Level >)
            return;

        while (true) {
            // As long as there are augmenting paths from the sink, add them.
            const auto path = GetAugmentingPath(levels, sink_node_id, graph, flow, source_vertices);
            if (path.empty())
                break;
            else {
                augment_flow(path);
                ++flow_increase;
            }
        }
    };

    std::for_each(border_sink_vertices.begin(), border_sink_vertices.end(), augment_all_paths);
    assert(flow_increase > 0);
    return flow_increase;
}

template<typename WeightType>
OptimizedKit::DinicMinCut<WeightType>::LevelGraph
OptimizedKit::DinicMinCut<WeightType>::ComputeLevelGraph(OldDirectedGraph <WeightType> &graph,
                                                         std::vector<VertexId> &border_source_vertices,
                                                         DinicMinCut::SourceSinkVertices &source_vertices,
                                                         DinicMinCut::SourceSinkVertices &sink_vertices,
                                                         DinicMinCut::FlowEdges &flow) {
    LevelGraph levels(graph.getVertexCount(), INVALID_VALUE < Level > );
    std::queue<VertexId> level_queue;

    // Enqueue border sources and their source neighbours and set their level to zero, allows levels as visited indicator.
    for (auto vertexId: border_source_vertices) {
        levels[vertexId] = 0;
        level_queue.push(vertexId);
        for (auto &edge: *graph.getOutgoingEdges(vertexId))
            if (source_vertices.count(edge.destination))
                levels[edge.destination] = 0;
        for (auto &edge: *graph.getIncomingEdges(vertexId))
            if (source_vertices.count(edge.destination))
                levels[edge.destination] = 0;
    }

    // Check if there is flow present on an edge.
    auto has_flow = [&](const VertexId from, const VertexId to) {
        return flow[from].find(to) != flow[from].end();
    };

    // Level relaxation step for a single vertex in BFS.
    auto relax_vertex = [&](const VertexId vertexId) {
        // Don't relax sink vertices.
        if (sink_vertices.count(vertexId))
            return;
        auto level = levels[vertexId] + 1;
        for (auto &edge: *graph.getOutgoingEdges(vertexId)) {
            auto destination = edge.destination;
            // Don't relax edges with flow on them.
            if (has_flow(vertexId, destination))
                continue;
            // Don't go back, only follow edges to new nodes.
            if (levels[destination] > level) {
                level_queue.push(destination);
                levels[destination] = level;
            }
        }
        for (auto &edge: *graph.getIncomingEdges(vertexId)) {
            auto destination = edge.destination;
            // Don't relax edges with flow on them.
            if (has_flow(vertexId, destination))
                continue;
            // Don't go back, only follow edges to new nodes.
            if (levels[destination] > level) {
                level_queue.push(destination);
                levels[destination] = level;
            }
        }
    };

    // Compute the levels of level graph using BFS.
    while (!level_queue.empty()) {
        relax_vertex(level_queue.front());
        level_queue.pop();
    }

    return levels;
}
*/