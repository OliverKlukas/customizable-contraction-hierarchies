#ifndef OPTIMIZEDKIT_CSV_READER_HPP
#define OPTIMIZEDKIT_CSV_READER_HPP

#include "graph/graph.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace OptimizedKit {
    class CsvReader {
    public:
        /**
         * @brief Reads a csv file and stores the data in a graph.
         *
         * @details Expects that the csv numbers the nodes randomly. Will reduce this numbering to 0 to n-1.
         *
         * @param filename - The name of the csv file.
         * @param graph - The graph to store the data in.
         * @param latitudes - The vector to store the latitudes in.
         * @param longitudes - The vector to store the longitudes in.
         * @param weights - The vector to store the weights in.
         */
        static void extractGraphFromCsv(const std::string &filename, OptimizedKit::Graph &graph, std::vector<float> &latitudes,
                                        std::vector<float> &longitudes, std::vector<float> &weights);

        /**
         * @brief Reads a csv file and stores the data in a graph.
         *
         * @details Expects that the csv numbers the nodes randomly. Will reduce this numbering to 0 to n-1.
         *
         * @param filename - The name of the csv file.
         * @param graph - The graph to store the data in.
         * @param latitudes - The vector to store the latitudes in.
         * @param longitudes - The vector to store the longitudes in.
         * @param weights - The vector to store the weights in.
         */
        static void extractGraphFromCsv(const std::string &filename, OptimizedKit::Graph &graph, std::vector<float> &latitudes,
                                        std::vector<float> &longitudes, std::vector<unsigned> &weights);

    };
}

#endif //OPTIMIZEDKIT_CSV_READER_HPP
