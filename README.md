# Efficient Customizable Contraction Hierarchies for shortest path queries in directed graphs

This repository contains a compact, modular C++ library for Customizable Contraction Hierarchies (CCH), inspired by and complementing the [original authors' publication](https://arxiv.org/abs/1402.0402) and [RoutingKit](https://github.com/RoutingKit/RoutingKit). Designed for flexibility, it allows for the seamless integration of any query algorithm or priority queue, making it highly adaptable for shortest path computation in a variety of settings. It's suited for use in large-scale road networks, accounting for turn costs and restrictions, and can also accommodate different graph structures such as graph databases. The library includes efficient implementations of priority queues, query algorithms, and graph data structures. For comprehensive details on Customizable Contraction Hierarchies, please refer to the [original publication](https://arxiv.org/abs/1402.0402) or [RoutingKit](https://github.com/RoutingKit/RoutingKit).

# Project Structure
```shell
.
├── external                                                      # External libraries      
├── LICENSE
├── optimizedkit
│   ├── CMakeLists.txt
│   ├── include                                                   # Header files                 
│   │   ├── customizable_contraction_hierarchy        
│   │   ├── graph
│   │   ├── graph_order_algorithms
│   │   ├── map
│   │   ├── path_finding_algorithms
│   │   ├── priority_queues
│   │   └── utils
│   ├── src                                                       # Source files             
│   │   ├── customizable_contraction_hierarchy
│   │   ├── graph
│   │   ├── graph_order_algorithms
│   │   ├── map
│   │   ├── path_finding_algorithms
│   │   ├── priority_queues
│   │   └── utils
│   └── tests                                                     # Test files               
└── README.md
```

# Dependencies
The library depends on the following libraries for testing purposes only, that should be cloned into the [external](./external/) folder automatically:

- [Google Test](https://github.com/google/googletest) 
- [RoutingKit](https://github.com/RoutingKit/RoutingKit)

# Development Guide

## Prerequisites
1. Install CMake (version 3.14 or higher) and a C++ compiler on your system.
2. Clone the repository:
    ```shell
    git clone https://github.com/OliverKlukas/customizable-contraction-hierarchies.git
    cd customizable-contraction-hierarchies/optimizedkit
    ```

## Build instructions
1. Create a build directory and navigate into it:
    ```shell
    cd customizable-contraction-hierarchies/optimizedkit
    mkdir build
    cd build
    ```

2. Build with `-DCMAKE_BUILD_TYPE=Debug` or `-DCMAKE_BUILD_TYPE=Release` depending on environment:
    1. Build library with all tests & examples:
        ```shell
        cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
        ```
    2. Build library with all tests:
        ```shell
        cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
        ```
    3. Build library with only RoutingKit independent tests:
        ```shell
        cmake -DCMAKE_BUILD_TYPE=Release -DTEST_AGAINST_ROUTING_KIT=OFF .. && cmake --build .
        ```
        
## Test instructions
1. After having followed the [build instructions](#build-instructions), test the code via:
    ```shell
    cd build
    ./tests/all_unit_tests
    ```

# License
This project is licensed under the [BSD 2-Clause License](LICENSE). Parts of the code that are directly derived from external libraries are marked accordingly.
