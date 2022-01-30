#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include <chrono>
#include <iomanip>
#include <thread>

#include "arraywrapper.hpp"
#include "sort.hpp"

Sort::Sort(ArrayWrapper *ary) {
    this->data = ary;
}

Sort::~Sort() { }

#ifdef __EMSCRIPTEN__
void Sort::em_tick() {
    auto tick_begin = std::chrono::system_clock::now();

    while (std::chrono::system_clock::now() - tick_begin < std::chrono::milliseconds(15)) {
        if (!finished && !paused)
            stats.elapsed = std::chrono::system_clock::now() - starttime;

        if (!paused) step();
    }
}
#endif

void Sort::loop() {
    while (true) {
        if (terminate) break;
        if (!finished && !paused)
            stats.elapsed = std::chrono::system_clock::now() - starttime;

        if (paused) std::this_thread::sleep_for(std::chrono::milliseconds(50));
        else step();
#ifndef __EMSCRIPTEN__
        std::this_thread::yield();
#endif
    }
}

void Sort::start() {
    terminate = false;
    paused = false;
#ifndef __EMSCRIPTEN__
    t = std::thread(&Sort::loop, this);
#endif
}

void Sort::pause() {
    pausetime = std::chrono::system_clock::now();
    paused = true;
}

void Sort::unpause() {
    starttime += std::chrono::system_clock::now() - pausetime;
    paused = false;
}

void Sort::toggle_pause() {
    if (paused) unpause();
    else pause();
}

void Sort::stop() {
#ifndef __EMSCRIPTEN__
    if (!t.joinable()) return;
    terminate = true;
    t.join();
#else
    terminate = true;
#endif
}

void Sort::reset() {
    finished = false;
    starttime = std::chrono::system_clock::now();
    stats.reset();
    data->stats.reset();
    _reset();
}

std::ostream& operator<<(std::ostream& o, const Sort::Stats& stats) {
    return o << std::left
        << "steps: " << std::setw(8) << stats.steps
        << " | elapsed: " << std::setw(8) << stats.elapsed.count()
        << std::setw(19) << " seconds"
        ;
}

