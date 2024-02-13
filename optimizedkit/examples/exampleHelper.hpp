#ifndef OPTIMIZEDKIT_EXAMPLE_HPP
#define OPTIMIZEDKIT_EXAMPLE_HPP

#include <random>
#include <iostream>
#include <filesystem>
#include <chrono>

namespace ExampleHelper {
    void printExampleDuration(const char *message, const std::chrono::high_resolution_clock::time_point &start,
                              const std::chrono::high_resolution_clock::time_point &end);

    std::string getOsmPath();

    bool validateOsmPath(const std::string &osm_path);
}

#endif //OPTIMIZEDKIT_EXAMPLE_HPP
