#include <algorithm>
#include <ostream>
#include <thread>
#include "arraywrapper.hpp"

ArrayWrapper::ArrayWrapper(int size) {
    _size = size;
    _data = new float[size];
    _indices = new float[size];

    for (int i = 0; i < size; i++) {
        _data[i] = (float)i;
        _indices[i] = (float)i;
    }
}

ArrayWrapper::~ArrayWrapper() {
    delete _data;
    delete _indices;
}

void ArrayWrapper::delay(int micros) {
    if (micros > 0) std::this_thread::sleep_for(std::chrono::microseconds(micros));
}

float ArrayWrapper::get(int idx) {
    stats.reads++;
    delay(read_delay);
    return _data[idx];
}

void ArrayWrapper::set(int idx, float val) {
    stats.writes++;
    delay(write_delay);
    _data[idx] = val;
}

void ArrayWrapper::swap(int i, int j) {
    stats.swaps++;
    stats.reads += 2;
    stats.writes += 2;
    delay(write_delay * 2 + read_delay * 2);
    std::swap(_data[i], _data[j]);
}

int ArrayWrapper::compare(int idx1, int idx2) {
    stats.compares++;
    stats.reads += 2;
    delay(read_delay * 2);
    return _data[idx1] - _data[idx2];
}

void ArrayWrapper::shuffle() {
    std::random_shuffle(_data, _data + _size - 1);
}

std::ostream& operator<<(std::ostream& o, const ArrayWrapper::Stats& stats) {
    return o << "Stats: r/w: " << stats.reads << '/' << stats.writes << "; compares: " << stats.compares << "; swaps: " << stats.swaps << ".";
}

