#include "sorts.hpp"

InsertionSort::InsertionSort(ArrayWrapper *ary) : Sort(ary) {
    ary->get_delay = [](int data_size) { return 1000000.0 / (data_size * data_size); };
}

void InsertionSort::step() {
    if (idx >= data->size()) {
        finished = true;
        return;
    }
    int m = idx;

    for (int i = 0; i < idx; i++) {
        if (data->compare(i, idx) >= 0) {
            m = i;
            break;
        }
    }

    for (int i = idx; i > m; i--) {
      data->swap(i, i-1);
    }

    idx++;
    stats.steps++;
}

void InsertionSort::_reset() {
    idx = 1;
}
