#include <thread>
#include <chrono>
#include <iomanip>

#include "arraywrapper.hpp"
#include "sort.hpp"

Sort::Sort(ArrayWrapper *ary) {
    this->data = ary;
}

Sort::~Sort() {
    
}

void Sort::loop() {
    while (true) {
        if (!finished && !paused) {
            stats.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - starttime).count();
        }
        if (terminate) return;
        if (paused) {
            delay(50, false);
            continue;
        }
        step();
        std::this_thread::yield();
    }
}

void Sort::delay(int millis, bool track_in_stats) {
    if (millis <= 0) return;
    auto now = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    auto end = std::chrono::high_resolution_clock::now();
    if(track_in_stats) stats.wait += std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - now).count();
}

void Sort::start() {
    terminate = false;
    paused = false;
    t = std::thread(&Sort::loop, this);
}

void Sort::pause() {
    pausetime = std::chrono::system_clock::now();
    paused = true;
}

void Sort::unpause() {
    starttime += std::chrono::high_resolution_clock::now() - pausetime;
    paused = false;
}

void Sort::toggle_pause() {
    if (paused) unpause();
    else pause();
}

void Sort::stop() {
    if (!t.joinable()) return;
    terminate = true;
    t.join();
}

void Sort::reset() {
    finished = false;
    starttime = std::chrono::system_clock::now();
    stats.reset();
    _reset();
}

std::ostream& operator<<(std::ostream& o, const Sort::Stats& stats) {
    return o << std::left
        << "steps: " << std::setw(8) << stats.steps
        << " | elapsed: " << std::setw(8) << stats.elapsed
        << " seconds | time waited: " << std::setw(8) << stats.wait
        << " seconds | waited for " << std::setw(8) << (stats.wait / stats.elapsed * 100) 
        << "% of the time"
        ;
}

