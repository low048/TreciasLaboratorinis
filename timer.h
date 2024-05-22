#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include "Vector.h"

class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;

    public:
        Timer() : start{std::chrono::high_resolution_clock::now()} {}
        void reset() {
            start = std::chrono::high_resolution_clock::now();
        }
        double elapsed() const {
            return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
        }
};

#endif
