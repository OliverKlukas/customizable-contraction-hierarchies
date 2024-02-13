#include "exampleHelper.hpp"

bool ExampleHelper::validateOsmPath(const std::string &osm_path) {
    if (osm_path.empty() || !std::filesystem::exists(osm_path) ||
        osm_path.substr(osm_path.find_last_of('.') + 1) != "csv")
        return false;
    return true;
}

void
ExampleHelper::printExampleDuration(const char *message, const std::chrono::high_resolution_clock::time_point &start,
                                    const std::chrono::high_resolution_clock::time_point &end) {
    std::chrono::duration<double, std::micro> duration = end - start;
    long seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::cout << message << " in: " << std::fixed << seconds << " seconds / " << milliseconds << " milliseconds / "
              << microseconds << " microseconds" << std::endl;
}

std::string ExampleHelper::getOsmPath() {
    std::string filename;
    char choice;
    while (true) {
        std::cout << "Please choose a file:" << std::endl;
        std::cout << "1. Munich" << std::endl;
        std::cout << "2. Specify your own..." << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
            case '1':
                filename = "test_data/munich.csv";
                break;
            case '2':
                std::cout << "Please enter the path to an OSM csv file: ";
                std::getline(std::cin, filename);
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
                continue;
        }
        if (validateOsmPath(filename))
            break;
        std::cout << "Invalid file path." << std::endl;
    }
    return filename;
}
