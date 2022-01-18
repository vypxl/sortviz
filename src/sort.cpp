#ifndef __EMSCRIPTEN__
    #include <thread>
#else
    #include <emscripten.h>
#endif
#include <iomanip>

#include "arraywrapper.hpp"
#include "sort.hpp"

Sort::Sort(ArrayWrapper *ary) {
    this->data = ary;
}

Sort::~Sort() { }

#ifdef __EMSCRIPTEN__
void Sort::em_tick() {
    if (!finished && !paused)
        stats.elapsed = SDL_GetTicks() - starttime;

    if (!paused) step();
}
#endif

void Sort::loop() {
    while (true) {
        if (terminate) break;
        if (!finished && !paused)
            stats.elapsed = SDL_GetTicks() - starttime;

        if (paused) delay(50, false);
        else step();
#ifndef __EMSCRIPTEN__
        std::this_thread::yield();
#endif
    }
}

void Sort::delay(unsigned int millis, bool track_in_stats) {
    if (millis <= 0) return;
    unsigned int begin = SDL_GetTicks();
#ifndef __EMSCRIPTEN__
    SDL_Delay(millis);
#else
    emscripten_sleep(millis);
#endif
    unsigned int end = SDL_GetTicks();
    if(track_in_stats) stats.wait += end - begin;
}

void Sort::start() {
    terminate = false;
    paused = false;
#ifndef __EMSCRIPTEN__
    t = std::thread(&Sort::loop, this);
#endif
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
    starttime = SDL_GetTicks();
    stats.reset();
    _reset();
}

std::ostream& operator<<(std::ostream& o, const Sort::Stats& stats) {
    return o << std::left
        << "steps: " << std::setw(8) << stats.steps
        << " | elapsed: " << std::setw(8) << (stats.elapsed / 1000.0)
        << std::setw(19) << "seconds"
        << "| time waited: " << std::setw(8) << (stats.wait / 1000.0)
        << " seconds | waited for " << std::setw(8) << (stats.wait / stats.elapsed * 100) 
        << "% of the time"
        ;
}

