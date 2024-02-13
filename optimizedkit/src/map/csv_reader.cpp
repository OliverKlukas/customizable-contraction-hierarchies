#include "map/csv_reader.hpp"

void OptimizedKit::CsvReader::extractGraphFromCsv(const std::string &filename, OptimizedKit::Graph &graph,
                                                  std::vector<float> &latitudes, std::vector<float> &longitudes,
                                                  std::vector<float> &weights) {
    // Open file and read.
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return;
    }

    // Read the first line and ignore it.
    std::string line;
    std::getline(file, line);
    std::vector<std::string> tokens;

    // Add vertices and save edges to when all vertices are added.
    std::string token;
    while (std::getline(file, line)) {
        tokens.clear();
        std::stringstream ss(line);
        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }
        if (std::stoi(tokens[0]) >= latitudes.size()) {
            longitudes.push_back(std::stof(tokens[3].substr(11, 9)));
            latitudes.push_back(std::stof(tokens[3].substr(21, 9)));
        }
        graph.tail.push_back(std::stoi(tokens[0]) - 1);
        graph.head.push_back(std::stoi(tokens[1]) - 1);
        weights.push_back(std::stof(tokens[2]));
    }
}

void OptimizedKit::CsvReader::extractGraphFromCsv(const std::string &filename, OptimizedKit::Graph &graph,
                                                  std::vector<float> &latitudes, std::vector<float> &longitudes,
                                                  std::vector<unsigned int> &weights) {
    // Open file and read.
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return;
    }

    // Read the first line and ignore it.
    std::string line;
    std::getline(file, line);
    std::vector<std::string> tokens;

    // Add vertices and save edges to when all vertices are added.
    std::string token;
    while (std::getline(file, line)) {
        tokens.clear();
        std::stringstream ss(line);
        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }
        if (std::stoi(tokens[0]) >= latitudes.size()) {
            longitudes.push_back(std::stof(tokens[3].substr(11, 9)));
            latitudes.push_back(std::stof(tokens[3].substr(21, 9)));
        }
        graph.tail.push_back(std::stoi(tokens[0]) - 1);
        graph.head.push_back(std::stoi(tokens[1]) - 1);
        weights.push_back(static_cast<unsigned int>(std::stoul(tokens[2])));
    }
}
