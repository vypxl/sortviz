#pragma once

#include <thread>
#include <mutex>

#include "config.hpp"

class Sort {
private:
    std::thread t;
    bool terminate = false;
    bool paused = false;

    void loop();

protected:
    void delay(int millis = DELAY);
    void swap(int i, int j);
    void shuffle();
    virtual void step() {};

public:
    float data[SIZE];
    float indices[SIZE];
    
    Sort();


    void start();
    void pause();
    void unpause();
    void toggle_pause();
    void stop();
    
    virtual void reset() {};
};
