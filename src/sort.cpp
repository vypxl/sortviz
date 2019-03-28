#include <thread>
#include <algorithm>

#include "sort.hpp"
#include "config.hpp"

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

void Sort::swap(int i, int j) {
    std::swap(data[i], data[j]);
}

Sort::Sort() {
    for (int i = 0; i < SIZE; i++) {
        data[i] = (float)i;
        indices[i] = (float)i;
    }
}

void Sort::shuffle() {
    std::random_shuffle(std::begin(data), std::end(data));
}

void Sort::start() {
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
    terminate = true;
    t.join();
}

