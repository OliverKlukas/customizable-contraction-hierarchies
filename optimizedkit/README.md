# OptimizedKit - Efficient customizable contraction hierarchies for shortest path queries

Code for efficient implementation of Customizable Contraction Hierachies as evaluated during my Master's thesis.

# Project Structure

The project is structured as follows:

```shell
.
├── CMakeLists.txt      
├── docs                # Documentation of the library
├── examples            # Example usages of the library
├── external            # External libraries
├── include             # Library public header files
├── LICENSE
├── README.md
├── src                 # Library source files
└── tests               # Library tests
```

# Documentation

For a quick start, see the [development guide](#development-guide) or test
the [example applications](./examples/).

# Dependencies

The library depends on the following libraries, see the [external](./external/) folder for details:

- Google Test
- RoutingKit

# Development Guide

## Prerequisites
1. Install CMake (version 3.14 or higher) and a C++ compiler on your system.
2. Clone the repository:
    ```shell
    git clone https://gitlab.db.in.tum.de/o.klukas/master-thesis
    cd master-thesis/optimizedkit
    ```

## Build instructions
1. Create a build directory and navigate into it:
    ```shell
    cd master-thesis/optimizedkit
    mkdir build
    cd build
    ```

2. Build with `-DCMAKE_BUILD_TYPE=Debug` or `-DCMAKE_BUILD_TYPE=Release` depending on environment:
    1. Build library with all tests & examples:
        ```shell
        cmake -DBUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
        ```
    2. Build library with all tests:
        ```shell
        cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
        ```
    3. Build library with only RoutingKit independent tests:
        ```shell
        cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=OFF -DTEST_AGAINST_ROUTING_KIT=OFF .. && cmake --build .
        ```

## Run examples instructions
1. After having followed the [build instructions](#build-instructions), run the example application to showcase OptimizedKit's usage:
    ```shell
    cd build
    ./example
    ```

## Test instructions
1. After having followed the [build instructions](#build-instructions), test the code via:
    ```shell
    cd build
    ./tests/all_unit_tests
    ```

# License

This project is licensed under the [BSD 2-Clause License](LICENSE).
