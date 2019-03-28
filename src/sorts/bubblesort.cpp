#include "sort.hpp"
#include "sorts.hpp"
#include "config.hpp"

void BubbleSort::step() {
    if (!swapped) return;
    
    for (int i = 0; i < n; i++) {
        if (data[i] > data[i+1]) {
            swap(i, i+1);
            swapped = true;
        } else {
            swapped = false;
        }
    }

    n--;
    delay();
}

void BubbleSort::reset() {
    shuffle();
    swapped = true;
    n = SIZE;
}
