#pragma once
#include <thread>
#include "arraywrapper.hpp"

class Sort {
private:
    std::thread t;
    bool terminate = false;
    bool paused = false;
    void loop();

protected:
    void delay(int millis);
    virtual void step() = 0;

public:
    Sort(ArrayWrapper *ary);
    ArrayWrapper *data;

    void start();
    void pause();
    void unpause();
    void toggle_pause();
    void stop();
    
    virtual void reset() = 0;
};
