# Tests independent of RoutingKit
set(ROUTING_KIT_INDEPENDENT_SOURCES
	graph/csv_reader_test.cpp
	priority_queues/binary_min_heap_test.cpp
	customizable_contraction_hierarchy/customizable_contraction_hierarchy_test.cpp
	path_finding_algorithms/bi_directional_dijkstra_test.cpp
	graph/graph_test.cpp
	utils/id_mapper_test.cpp
	utils/permutation_test.cpp
	utils/vector_helper_test.cpp
	test_utils/utils.hpp
	utils/math_test.cpp
	priority_queues/pairing_min_heap_test.cpp)

# Tests against RoutingKit
set(ROUTING_KIT_DEPENDENT_SOURCES
	customizable_contraction_hierarchy/cch_preprocessor_test.cpp
	customizable_contraction_hierarchy/cch_customizer_test.cpp
	customizable_contraction_hierarchy/cch_query_test.cpp
	customizable_contraction_hierarchy/cch_triangle_enumeration_test.cpp)

# Build all unit tests together
option(TEST_AGAINST_ROUTING_KIT "Test against RoutingKit" ON)
message(STATUS "TEST_AGAINST_ROUTING_KIT: ${TEST_AGAINST_ROUTING_KIT}")
if(TEST_AGAINST_ROUTING_KIT)
  add_executable(all_unit_tests ${ROUTING_KIT_INDEPENDENT_SOURCES}
                                ${ROUTING_KIT_DEPENDENT_SOURCES})
  target_link_libraries(all_unit_tests gtest gtest_main gmock optimizedkit ${ROUTING_KIT_LIBRARIES})
else()
  add_executable(all_unit_tests ${ROUTING_KIT_INDEPENDENT_SOURCES})
  target_link_libraries(all_unit_tests gtest gtest_main gmock optimizedkit)
endif()

# Build cch stage tests as separate executables
if(TEST_AGAINST_ROUTING_KIT)
	add_executable(cch_preprocessor_tests
				   customizable_contraction_hierarchy/cch_preprocessor_test.cpp
				   test_utils/utils.hpp
	)
	target_link_libraries(cch_preprocessor_tests gtest gtest_main gmock optimizedkit ${ROUTING_KIT_LIBRARIES})
	add_executable(cch_customizer_tests
				   customizable_contraction_hierarchy/cch_customizer_test.cpp
				   test_utils/utils.hpp
	)
	target_link_libraries(cch_customizer_tests gtest gtest_main gmock optimizedkit ${ROUTING_KIT_LIBRARIES})
	add_executable(cch_query_tests
				   customizable_contraction_hierarchy/cch_query_test.cpp
				   test_utils/utils.hpp
	)
	target_link_libraries(cch_query_tests gtest gtest_main gmock optimizedkit ${ROUTING_KIT_LIBRARIES})
endif()

# Include the library's header directory
target_include_directories(all_unit_tests
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Copy the test_data directory to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_data
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../)

enable_testing()
include(GoogleTest)
gtest_discover_tests(all_unit_tests)
