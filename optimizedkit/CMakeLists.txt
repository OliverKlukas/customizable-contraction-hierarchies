cmake_minimum_required(VERSION 3.14)
project(optimizedkit)
set(CMAKE_CXX_STANDARD 20)

# Include RoutingKit
include_directories(${CMAKE_SOURCE_DIR}/../external/RoutingKit/include)
set(ROUTING_KIT_LIBRARIES ${CMAKE_SOURCE_DIR}/../external/RoutingKit/lib/libroutingkit.so)

# Reference googletest
option(BUILD_TESTS "Build tests for library" ON)
if(BUILD_TESTS)
	add_subdirectory(${CMAKE_SOURCE_DIR}/../external/GoogleTest ${CMAKE_BINARY_DIR}/GoogleTest)
endif()

# Build optimizedkit library
set(LIBRARY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBRARY_SOURCES
	src/priority_queues/binary_min_heap.tpp
	src/customizable_contraction_hierarchy/customizable_contraction_hierarchy.tpp
	src/graph_order_algorithms/random_order.tpp
	src/map/csv_reader.cpp
	include/priority_queues/binary_min_heap.hpp
	include/graph_order_algorithms/random_order.hpp
	include/customizable_contraction_hierarchy/customizable_contraction_hierarchy.hpp
	include/utils/types.hpp
	include/map/csv_reader.hpp
	include/graph/graph.hpp
	src/graph/graph.cpp
	include/utils/constants.hpp
	include/customizable_contraction_hierarchy/cch_preprocessor.hpp
	include/customizable_contraction_hierarchy/cch_customizer.hpp
	include/customizable_contraction_hierarchy/cch_query.hpp
	src/customizable_contraction_hierarchy/cch_preprocessor.cpp
	src/customizable_contraction_hierarchy/cch_customizer.tpp
	src/customizable_contraction_hierarchy/cch_query.tpp
	include/utils/enums.hpp
	include/utils/permutation.hpp
	include/utils/id_mapper.hpp
	src/utils/id_mapper.cpp
	include/utils/vector_helper.hpp
	src/utils/permutation.cpp
	src/utils/vector_helper.tpp
	src/utils/permutation.tpp
	include/graph/cch_graph.hpp
	src/graph/cch_graph.tpp
	include/utils/math.hpp
	include/utils/graph_helper.hpp
	src/utils/graph_helper.cpp
	include/map/coordinate_to_vertex_mapper.hpp
	src/map/coordinate_to_vertex_mapper.cpp
	include/path_finding_algorithms/dijkstra.hpp
	src/path_finding_algorithms/dijkstra.tpp
	src/path_finding_algorithms/bi_directional_dijkstra.tpp
	include/path_finding_algorithms/bi_directional_dijkstra.hpp
	include/customizable_contraction_hierarchy/cch_triangle_enumeration.hpp
	src/customizable_contraction_hierarchy/cch_triangle_enumeration.tpp
	include/priority_queues/pairing_min_heap.hpp
	include/priority_queues/pairing_min_heap.hpp
	src/priority_queues/pairing_min_heap.tpp
	include/priority_queues/abstract_heap.hpp
)
add_library(${PROJECT_NAME} ${LIBRARY_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBRARY_INCLUDE_DIR})

# Compile options for Release configuration
target_compile_options(${PROJECT_NAME} PRIVATE
					   $<$<CONFIG:Release>:-O3>
					   $<$<CONFIG:Release>:-g>
					   #$<$<CONFIG:Release>:-ffast-math>
)

# Compile options for Debug configuration
target_compile_options(${PROJECT_NAME} PRIVATE
					   $<$<CONFIG:Debug>:-O0>
					   $<$<CONFIG:Debug>:-g>
					   $<$<CONFIG:Debug>:-UNDEBUG>
)

# Include tests
if(BUILD_TESTS)
	add_subdirectory(tests)
endif ()
