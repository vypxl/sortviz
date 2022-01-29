#ifndef __sort_hpp_
#define __sort_hpp_

#ifndef __EMSCRIPTEN__
    #include <thread>
#endif
#include <SDL.h>
#include <chrono>

#include "arraywrapper.hpp"

class Sort {
private:
#ifndef __EMSCRIPTEN__
    std::thread t;
#endif
    std::chrono::system_clock::time_point starttime;
    std::chrono::system_clock::time_point pausetime;
    bool terminate = false;
    bool paused = false;
    void loop();

protected:
    bool finished = false;
    virtual void step() = 0;
    virtual void _reset() = 0;

public:
    struct Stats{
        unsigned int steps;
        std::chrono::duration<double> elapsed;

        Stats() {
            reset();
        }

        void reset() {
            steps = 0; elapsed = std::chrono::seconds(0);
        }
    };

    Sort(ArrayWrapper *ary);
    virtual ~Sort();
    Stats stats;
    ArrayWrapper *data;

    void start();
#ifdef __EMSCRIPTEN__
    void em_tick();
#endif
    void pause();
    void unpause();
    void toggle_pause();
    void stop();
    
    void reset();
};

std::ostream& operator<<(std::ostream& o, const Sort::Stats& stats);

#endif
