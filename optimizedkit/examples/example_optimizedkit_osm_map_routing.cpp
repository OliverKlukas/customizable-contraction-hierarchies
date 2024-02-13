#include "exampleHelper.hpp"
#include "customizable_contraction_hierarchy/customizable_contraction_hierarchy.hpp"
#include "map/csv_reader.hpp"
#include "routingkit/nested_dissection.h"
#include "routingkit/geo_position_to_node.h"

using namespace OptimizedKit;

int main(){
    std::cout << "I. PREPARATIONS" << std::endl;
    OptimizedKit::Graph graph;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<unsigned> weights;
    auto filename = ExampleHelper::getOsmPath();
    auto startTime = std::chrono::high_resolution_clock::now();
    CsvReader::extractGraphFromCsv(filename, graph, latitudes, longitudes, weights);
    auto endTime = std::chrono::high_resolution_clock::now();
    ExampleHelper::printExampleDuration("graph, map data & weights read", startTime, endTime);

    startTime = std::chrono::high_resolution_clock::now();
    // TODO: replace with OptimizedKit implementation
    auto order = RoutingKit::compute_nested_node_dissection_order_using_inertial_flow(latitudes.size(),
                                                                                      graph.tail, graph.head,
                                                                                      latitudes, longitudes);
    endTime = std::chrono::high_resolution_clock::now();
    ExampleHelper::printExampleDuration("order constructed", startTime, endTime);

    std::cout << "II. CUSTOMIZABLE CONTRACTION HIERARCHY CONSTRUCTION" << std::endl;
    startTime = std::chrono::high_resolution_clock::now();
    OptimizedKit::CustomizableContractionHierarchy cch(order, graph, weights);
    endTime = std::chrono::high_resolution_clock::now();
    ExampleHelper::printExampleDuration("cch with all three phases constructed", startTime, endTime);

    while (true) {
        std::cout << "III: QUERYING" << std::endl;
        // TODO: replace with OptimizedKit implementation
        RoutingKit::GeoPositionToNode map_geo_position(latitudes, longitudes);
        float sourceLat, sourceLon, targetLat, targetLon;
        VertexId source = INVALID_VALUE<VertexId>, target = INVALID_VALUE<VertexId>;
        while (source == RoutingKit::invalid_id) {
            std::cout << "Enter source latitude (e.g. 11.5073 in Munich): ";
            std::cin >> sourceLat;
            std::cin.ignore();
            std::cout << "Enter source longitude (e.g. 48.1547 in Munich): ";
            std::cin >> sourceLon;
            std::cin.ignore();
            std::cout << "Finding nearest node..." << std::endl;
            source = map_geo_position.find_nearest_neighbor_within_radius(sourceLat, sourceLon, 1000).id;
            if (source == RoutingKit::invalid_id)
                std::cout << "Invalid input: No node within 1000m from position" << std::endl;
            else
                std::cout << "Source node: " << source << std::endl;
        }
        while (target == RoutingKit::invalid_id) {
            std::cout << "Enter target latitude (e.g. 11.6668 in Munich): ";
            std::cin >> targetLat;
            std::cin.ignore();
            std::cout << "Enter target longitude (e.g. 48.1150 in Munich): ";
            std::cin >> targetLon;
            std::cin.ignore();
            std::cout << "Finding nearest node..." << std::endl;
            target = map_geo_position.find_nearest_neighbor_within_radius(targetLat, targetLon, 1000).id;
            if (target == RoutingKit::invalid_id)
                std::cout << "Invalid input: No node within 1000m from position" << std::endl;
            else
                std::cout << "Target node: " << target << std::endl;
        }

        startTime = std::chrono::high_resolution_clock::now();
        cch.run(source, target);
        endTime = std::chrono::high_resolution_clock::now();
        ExampleHelper::printExampleDuration("Query result found", startTime, endTime);

        std::cout << "IV. RESULTS" << std::endl;
        startTime = std::chrono::high_resolution_clock::now();
        auto tripWeight = cch.getQueryWeight();
        endTime = std::chrono::high_resolution_clock::now();
        ExampleHelper::printExampleDuration("Trip weight retrieved", startTime, endTime);

        startTime = std::chrono::high_resolution_clock::now();
        auto vertexPath = cch.getVertexPath();
        endTime = std::chrono::high_resolution_clock::now();
        ExampleHelper::printExampleDuration("Vertex path reconstructed", startTime, endTime);

        startTime = std::chrono::high_resolution_clock::now();
        auto arcPath = cch.getEdgePath();
        endTime = std::chrono::high_resolution_clock::now();
        ExampleHelper::printExampleDuration("Arc path reconstructed", startTime, endTime);

        std::cout << "Found a route from " << source << " to " << target << std::endl;
        std::cout << "Shortest distance: " << tripWeight << " meters" << std::endl;
        std::cout << "Shortest vertex path: ";
        for (auto vertex: vertexPath) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
        std::cout << "Shortest arc path: ";
        for (auto arc: arcPath) {
            std::cout << arc << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}