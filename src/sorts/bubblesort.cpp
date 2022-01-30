#include "sorts.hpp"

BubbleSort::BubbleSort(ArrayWrapper *ary) : Sort(ary) {
    ary->get_delay = [](int data_size) { return 5000000.0 / (data_size * data_size); };
}

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
}

void BubbleSort::_reset() {
    swapped = true;
    n = data->size() - 1;
}

