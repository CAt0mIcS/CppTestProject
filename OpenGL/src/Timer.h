#pragma once

#include <chrono>
#include <functional>
#include <iostream>


class Timer {
public:
    Timer(std::function<void(std::chrono::time_point<std::chrono::steady_clock>& startTime, std::chrono::time_point<std::chrono::steady_clock>& endTime)> func) {
        this->func = func;

        startTime = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        endTime = std::chrono::high_resolution_clock::now();
        func(startTime, endTime);
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::time_point<std::chrono::steady_clock> endTime;
    std::function<void(std::chrono::time_point<std::chrono::steady_clock>& startTime, std::chrono::time_point<std::chrono::steady_clock>& endTime)> func;
};


inline void PrintTimer(std::chrono::time_point<std::chrono::steady_clock>& startTime, std::chrono::time_point<std::chrono::steady_clock>& endTime) {
    std::chrono::duration<float> duration;

    duration = endTime - startTime;
    float us = duration.count() * 1000 * 1000;

    std::cout << "Timer took " << us << "us\n";

}