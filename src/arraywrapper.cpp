#include <algorithm>
#include <ostream>
#include <iomanip>
#include <thread>
#include "arraywrapper.hpp"

ArrayWrapper::ArrayWrapper(int size) {
    _size = size;
    stats.size = size;
    _data = new float[size];
    _indices = new float[size];

    for (int i = 0; i < size; i++) {
        _data[i] = (float)i;
        _indices[i] = (float)i;
    }
}

ArrayWrapper::~ArrayWrapper() {
    delete[] _data;
    delete[] _indices;
}

float ArrayWrapper::get(int idx) {
    delay();
    stats.reads++;

    return _data[idx];
}

void ArrayWrapper::set(int idx, float val) {
    delay();
    stats.writes++;
    _data[idx] = val;
}

void ArrayWrapper::swap(int i, int j) {
    delay();
    stats.swaps++;

    float tmp = get(i);
    set(i, get(j));
    set(j, tmp);
}

int ArrayWrapper::compare(int idx1, int idx2) {
    delay();
    stats.compares++;

    return get(idx1) - get(idx2);
}

void ArrayWrapper::shuffle() {
    std::random_shuffle(_data, _data + _size - 1);
}

void ArrayWrapper::delay() {
    if (_size >= 65536) return;
    waitQueue += get_delay(_size);
    if (waitQueue <= 60) return;

    int waitTime = static_cast<int>(waitQueue);
    auto begin = std::chrono::system_clock::now();

    std::this_thread::sleep_for(std::chrono::microseconds(waitTime));

    auto end = std::chrono::system_clock::now();
    stats.waited += end - begin;
    waitQueue = 0;
}

std::ostream& operator<<(std::ostream& o, const ArrayWrapper::Stats& stats) {
    return o << std::left
        << "size:  " << std::setw(8) << stats.size
        << " | reads / writes: " << std::setw(8) << stats.reads << " / " << std::setw(8) << stats.writes
        << " | compares: " << std::setw(19) << stats.compares
        << " | swaps: " << std::setw(8) << stats.swaps
        ;
}

