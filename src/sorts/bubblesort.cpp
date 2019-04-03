#include "sorts.hpp"

void BubbleSort::step() {
    if (!swapped) {
        finished = true;
        return;
    }

    swapped = false;
    for (int i = 0; i < n; i++) {
        if (data->compare(i, i+1) > 0) {
            data->swap(i, i+1);
            swapped = true;
        }
    }

    n--;
    stats.steps++;
    delay(10000 / data->size());
}

void BubbleSort::_reset() {
    swapped = true;
    n = data->size();
}

