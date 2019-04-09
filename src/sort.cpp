#include <thread>
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
            stats.elapsed = SDL_GetTicks() - starttime;
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

void Sort::delay(unsigned int millis, bool track_in_stats) {
    if (millis <= 0) return;
    unsigned int begin = SDL_GetTicks();
    SDL_Delay(millis);
    unsigned int end = SDL_GetTicks();
    if(track_in_stats) stats.wait += end - begin;
}

void Sort::start() {
    terminate = false;
    paused = false;
    t = std::thread(&Sort::loop, this);
}

void Sort::pause() {
    pausetime = SDL_GetTicks();
    paused = true;
}

void Sort::unpause() {
    starttime += SDL_GetTicks() - pausetime;
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
    starttime = SDL_GetTicks();
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

