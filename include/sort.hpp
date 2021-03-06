#ifndef __sort_hpp_
#define __sort_hpp_

#include <thread>
#include <SDL/SDL_timer.h>

#include "arraywrapper.hpp"

class Sort {
public:
    struct Stats{
        unsigned int steps;
        double elapsed;
        double wait;

        Stats() {
            reset();
        }

        void reset() {
            steps = 0; elapsed = 0; wait = 0;
        }
    };
private:
    std::thread t;
    unsigned int starttime;
    unsigned int pausetime;
    bool terminate = false;
    bool paused = false;
    void loop();

protected:
    bool finished = false;
    void delay(unsigned int millis, bool track_in_stats = true);
    virtual void step() = 0;
    virtual void _reset() = 0;

public:
    Sort(ArrayWrapper *ary);
    ~Sort();
    Stats stats;
    ArrayWrapper *data;

    void start();
    void pause();
    void unpause();
    void toggle_pause();
    void stop();
    
    void reset();
};

std::ostream& operator<<(std::ostream& o, const Sort::Stats& stats);

#endif
