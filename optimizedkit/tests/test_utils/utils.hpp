#ifndef OPTIMIZEDKIT_UTILS_HPP
#define OPTIMIZEDKIT_UTILS_HPP

#include <chrono>
#include <iostream>

inline
void printDuration(const char *message, const std::chrono::high_resolution_clock::time_point &start,
                   const std::chrono::high_resolution_clock::time_point &end){
    std::chrono::duration<double, std::micro> duration = end - start;
    long seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::cout << message << " in: " << std::fixed << seconds << " seconds / "
              << milliseconds << " milliseconds / "
              << microseconds << " microseconds" << std::endl;
}

#endif //OPTIMIZEDKIT_UTILS_HPP
