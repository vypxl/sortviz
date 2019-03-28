#include "sort.hpp"
#include "sorts.hpp"
#include "config.hpp"

void SelectionSort::step() {
    if (idx >= SIZE) return;
    int m = idx;
    
    for (int i = idx; i < SIZE; i++) {
        if (data[i] < data[m]) {
            m = i;
        }
    }

    swap(idx, m);

    idx++;
    delay();
}

void SelectionSort::reset() {
    shuffle();
    idx = 0;
}
