#include <gtest/gtest.h>
#include "graph/graph.hpp"
#include "map/csv_reader.hpp"
#include "routingkit/nested_dissection.h"
#include "routingkit/customizable_contraction_hierarchy.h"
#include "routingkit/contraction_hierarchy.h"
#include "utils/vector_helper.hpp"
#include "routingkit/geo_position_to_node.h"
#include "customizable_contraction_hierarchy/cch_preprocessor.hpp"
#include "customizable_contraction_hierarchy/cch_customizer.hpp"
#include "customizable_contraction_hierarchy/cch_query.hpp"
#include "../test_utils/utils.hpp"


TEST(ExampleGenTest, ExampleGen_Test_ForLatexPlotGaertnerplatz) {
// Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes = {20, 10, 0, 30, 15, 5, 30, 20, 15};
    std::vector<float> longitudes = {0, 5, 10, 10, 15, 20, 20, 30, 30};
    graph.addEdge(0, 1);
    graph.addEdge(0, 3);
    graph.addEdge(1, 0);
    graph.addEdge(1, 2);
    graph.addEdge(2, 1);
    graph.addEdge(2, 4);
    graph.addEdge(2, 5);
    graph.addEdge(3, 6);
    graph.addEdge(4, 1);
    graph.addEdge(4, 3);
    graph.addEdge(5, 4);
    graph.addEdge(5, 8);
    graph.addEdge(6, 4);
    graph.addEdge(6, 7);
    graph.addEdge(7, 4);
    graph.addEdge(7, 8);
    std::vector<unsigned> weights = {2, 10, 3, 5, 1, 1, 2, 8, 7, 2, 10, 3, 5, 1, 1, 2};
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(), graph.tail,
                                                                                      graph.head, latitudes,
                                                                                      longitudes);
    graph.vertexCount = latitudes.size();

// Prerequisites
    std::cout << "Input graph: " << std::endl;
    std::cout << "tail: ";
    for (unsigned int i: graph.tail) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "head: ";
    for (unsigned int i: graph.head) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "weights: ";
    for (unsigned int weight: weights) {
        std::cout << weight << " ";
    }
    std::cout << std::endl;
    std::cout << "NDO order: ";
    for (unsigned int i: order) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Latitudes: ";
    for (float latitude: latitudes) {
        std::cout << latitude << " ";
    }
    std::cout << std::endl;
    std::cout << "Longitudes: ";
    for (float longitude: longitudes) {
        std::cout << longitude << " ";
    }
    std::cout << std::endl;

    std::cout << "Adjacency list: ", graph.

            createAdjacencyIndices();

    for (unsigned int adjacencyIndice: graph.adjacencyIndices) {
        std::cout << adjacencyIndice << " ";
    }
    std::cout << std::endl;

// Also compute ifco and cho.
    auto ch = RoutingKit::ContractionHierarchy::build(longitudes.size(), graph.tail, graph.head, weights);
    std::cout << "CHO order: ";
    for (unsigned int i: ch.order) {
        std::cout << i << " ";
    }
    std::cout << std::endl;


// Preprocessing
    OptimizedKit::CchPreprocessor preprocessor(order, graph);
    std::cout << "Upwards graph: " << std::endl;
    std::cout << "tail: ";
    for (unsigned int i: preprocessor.upwardsGraph.tail) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "head: ";
    for (unsigned int i: preprocessor.upwardsGraph.head) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "rank: ";
    for (unsigned int i: preprocessor.rank) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

// Customization
    OptimizedKit::CchCustomizer customizer(preprocessor, weights);
    customizer.

            baseCustomization();

    std::cout << "Basic forward weights: ";
    for (unsigned int forwardWeight: customizer.forwardWeights) {
        std::cout << forwardWeight << " ";
    }
    std::cout << std::endl;
    std::cout << "Basic backward weights: ";
    for (unsigned int backwardWeight: customizer.backwardWeights) {
        std::cout << backwardWeight << " ";
    }
    std::cout << std::endl;

    customizer.

            perfectCustomization();

    std::cout << "Perfect Upwards graph: " << std::endl;
    std::cout << "tail: ";
    for (unsigned int i: preprocessor.upwardsGraph.tail) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "head: ";
    for (unsigned int i: preprocessor.upwardsGraph.head) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "Perfect forward weights: ";
    for (unsigned int forwardWeight: customizer.forwardWeights) {
        std::cout << forwardWeight << " ";
    }
    std::cout << std::endl;
    std::cout << "Perfect backward weights: ";
    for (unsigned int backwardWeight: customizer.backwardWeights) {
        std::cout << backwardWeight << " ";
    }
    std::cout << std::endl;

// Update weights
/*
std::vector<OptimizedKit::EdgeId> updateEdges = {1, 6, 8, 9};
weights[1] = 1;
weights[6] = 2;
weights[8] = 3;
weights[9] = 4;
customizer.update(updateEdges);
std::cout << "Updated forward weights: ";
for (unsigned int forwardWeight : customizer.forwardWeights) {
    std::cout << forwardWeight << " ";
}
std::cout << std::endl;
std::cout << "Updated backward weights: ";
for (unsigned int backwardWeight : customizer.backwardWeights) {
    std::cout << backwardWeight << " ";
}
std::cout << std::endl;*/

// Query a path
    OptimizedKit::CchQuery query(customizer);
    OptimizedKit::VertexId source = 0;
    OptimizedKit::VertexId target = 7;
    query.run(source, target);
    auto distance = query.getQueryWeight();
    std::cout << "Distance: " << distance << std::endl;

    auto vertexPath = query.getVertexPath();
    std::cout << "Vertex path: ";
    for (unsigned int vertex: vertexPath) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    auto edgePath = query.getEdgePath();
    std::cout << "Edge path: ";
    for (unsigned int edge: edgePath) {
        std::cout << edge << " ";
    }
    std::cout << std::endl;

// Assert
    assert(true);
}

TEST(ExampleGenTest, ExampleGen_Test_ForLatexEvaluationOrders) {
// Arrange
    OptimizedKit::Graph graph;
    std::vector<float> latitudes = {20, 10, 0, 30, 15, 5, 30, 20, 15};
    std::vector<float> longitudes = {0, 5, 10, 10, 15, 20, 20, 30, 30};
    graph.addEdge(0, 1);
    graph.addEdge(0, 3);
    graph.addEdge(1, 0);
    graph.addEdge(1, 2);
    graph.addEdge(2, 1);
    graph.addEdge(2, 4);
    graph.addEdge(2, 5);
    graph.addEdge(3, 6);
    graph.addEdge(4, 1);
    graph.addEdge(4, 3);
    graph.addEdge(5, 4);
    graph.addEdge(5, 8);
    graph.addEdge(6, 4);
    graph.addEdge(6, 7);
    graph.addEdge(7, 4);
    graph.addEdge(7, 8);
    std::vector<unsigned> weights = {2, 10, 3, 5, 1, 1, 2, 8, 7, 2, 10, 3, 5, 1, 1, 2};
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(), graph.tail,
                                                                                      graph.head, latitudes,
                                                                                      longitudes);
    graph.vertexCount = latitudes.size();

    std::cout << "Input graph: " << std::endl;
    std::cout << "tail: ";
    for (unsigned int i: graph.tail) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "head: ";
    for (unsigned int i: graph.head) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

// NDO
    std::cout << "NDO" << std::endl;
    OptimizedKit::CchPreprocessor ndoPreprocessor(order, graph);
    std::cout << "NDO Upwards graph: " << std::endl;
    std::cout << "NDO tail: ";
    for (unsigned int i: ndoPreprocessor.upwardsGraph.tail) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "NDO head: ";
    for (unsigned int i: ndoPreprocessor.upwardsGraph.head) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "NDO number of edges: " << ndoPreprocessor.upwardsGraph.head.size() << std::endl;
    std::cout << "NDO rank: ";
    for (unsigned int i: ndoPreprocessor.rank) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "NDO order: ";
    for (unsigned int i: order) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

// CHO
    auto ch = RoutingKit::ContractionHierarchy::build(longitudes.size(), graph.tail, graph.head, weights);
    OptimizedKit::CchPreprocessor choPreprocessor(ch.order, graph);
    std::cout << "CHO Upwards graph: " << std::endl;
    std::cout << "CHO tail: ";
    for (unsigned int i: choPreprocessor.upwardsGraph.tail) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "CHO head: ";
    for (unsigned int i: choPreprocessor.upwardsGraph.head) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "CHO number of edges: " << choPreprocessor.upwardsGraph.head.size() << std::endl;
    std::cout << "CHO rank: ";
    for (unsigned int i: choPreprocessor.rank) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "CHO order: ";
    for (unsigned int i: ch.order) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

// IFCO
    std::string ifcoFile = "../../tests/test_data/example_graph/ifco";
    std::vector<unsigned> ifco;
    std::ifstream in(ifcoFile, std::ios_base::binary);
    if (!in) {
        std::cerr << "Cannot open file: " << ifcoFile << std::endl;
    }
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    size_t numElements = size / sizeof(unsigned);
    ifco.resize(numElements);
    in.read(reinterpret_cast<char *>(ifco.data()), size);
    in.close();

    OptimizedKit::CchPreprocessor ifcoPreprocessor(ifco, graph);
    std::cout << "IFCO Upwards graph: " << std::endl;
    std::cout << "IFCO tail: ";
    for (unsigned int i: ifcoPreprocessor.upwardsGraph.tail) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "IFCO head: ";
    for (unsigned int i: ifcoPreprocessor.upwardsGraph.head) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "IFCO number of edges: " << ifcoPreprocessor.upwardsGraph.head.size() << std::endl;
    std::cout << "IFCO rank: ";
    for (unsigned int i: ifcoPreprocessor.rank) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "IFCO order: ";
    for (unsigned int i: ifco) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

// Assert
    assert(true);
}

TEST(ExampleGenTest, ExampleGen_Test_WriteGraphExample) {
    OptimizedKit::Graph graph;
    std::vector<float> latitudes = {20, 10, 0, 30, 15, 5, 30, 20, 15};
    std::vector<float> longitudes = {0, 5, 10, 10, 15, 20, 20, 30, 30};
    graph.addEdge(0, 1);
    graph.addEdge(0, 3);
    graph.addEdge(1, 0);
    graph.addEdge(1, 2);
    graph.addEdge(2, 1);
    graph.addEdge(2, 4);
    graph.addEdge(2, 5);
    graph.addEdge(3, 6);
    graph.addEdge(4, 1);
    graph.addEdge(4, 3);
    graph.addEdge(5, 4);
    graph.addEdge(5, 8);
    graph.addEdge(6, 4);
    graph.addEdge(6, 7);
    graph.addEdge(7, 4);
    graph.addEdge(7, 8);
    std::vector<unsigned> weights = {2, 10, 3, 5, 1, 1, 2, 8, 7, 2, 10, 3, 5, 1, 1, 2};
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(), graph.tail,
                                                                                      graph.head, latitudes,
                                                                                      longitudes);
    graph.vertexCount = latitudes.size();
    graph.createAdjacencyIndices();

    // print out the current directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "Current working dir: %s\n", cwd);
    else
        perror("getcwd() error");
    std::string baseFolder = "../../tests/test_data/example_graph";

    std::string latitudeFile = baseFolder + "/latitude";
    std::ofstream out(latitudeFile, std::ios_base::binary);
    out.write(reinterpret_cast<const char *>(latitudes.data()), latitudes.size() * sizeof(float));
    out.close();

    std::string longitudeFile = baseFolder + "/longitude";
    out.open(longitudeFile, std::ios_base::binary);
    out.write(reinterpret_cast<const char *>(longitudes.data()), longitudes.size() * sizeof(float));
    out.close();

    std::string weightFile = baseFolder + "/weight";
    out.open(weightFile, std::ios_base::binary);
    out.write(reinterpret_cast<const char *>(weights.data()), weights.size() * sizeof(unsigned));
    out.close();

    std::string headFile = baseFolder + "/head";
    out.open(headFile, std::ios_base::binary);
    out.write(reinterpret_cast<const char *>(graph.head.data()), graph.head.size() * sizeof(unsigned));
    out.close();

    std::string firstOutFile = baseFolder + "/first_out";
    out.open(firstOutFile, std::ios_base::binary);
    out.write(reinterpret_cast<const char *>(graph.adjacencyIndices.data()),
              graph.adjacencyIndices.size() * sizeof(unsigned));
    out.close();

    std::string tailFile = baseFolder + "/tail";
    out.open(tailFile, std::ios_base::binary);
    out.write(reinterpret_cast<const char *>(graph.tail.data()), graph.tail.size() * sizeof(unsigned));
    out.close();
}