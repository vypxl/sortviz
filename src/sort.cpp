#include <thread>
#include <chrono>

#include "arraywrapper.hpp"
#include "sort.hpp"

Sort::Sort(ArrayWrapper *ary) {
    this->data = ary;
}

Sort::~Sort() {
    stop();
}

void Sort::loop() {
    while (true) {
        if (!finished) {
            stats.elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - starttime).count();
        }
        if (terminate) return;
        if (paused) {
            delay(50);
            continue;
        }
        step();
    }
}

void Sort::delay(int millis) {
    if (millis <= 0) return;
    auto now = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    auto end = std::chrono::high_resolution_clock::now();
    stats.wait += std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - now).count();
}

void Sort::start() {
    terminate = false;
    paused = false;
    t = std::thread(&Sort::loop, this);
}

void Sort::pause() {
    paused = true;
}

void Sort::unpause() {
    paused = false;
}

void Sort::toggle_pause() {
    paused = paused ? false : true;
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
    return o
        << "steps: " << stats.steps
        << "; elapsed: " << stats.elapsed
        << " seconds, time waited: " << stats.wait
        << " seconds, waited for " << (stats.wait / stats.elapsed * 100) 
        << "% of the time"
        ;
}

