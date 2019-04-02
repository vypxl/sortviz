#include <thread>
#include <iostream>

#include "sort.hpp"
#include "arraywrapper.hpp"

Sort::Sort(ArrayWrapper *ary) {
    this->data = ary;
}

Sort::~Sort() {
    stop();
}

void Sort::loop() {
    while (true) {
        if (terminate) return;
        if (paused) {
            delay(50);
            continue;
        }
        step();
    }
}

void Sort::delay(int millis) {
    if (millis > 0) std::this_thread::sleep_for(std::chrono::milliseconds(millis));
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

