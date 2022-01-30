#include "sorts.hpp"

SelectionSort::SelectionSort(ArrayWrapper *ary) : Sort(ary) {
    ary->get_delay = [](int data_size) { return 1000000.0 / (data_size * data_size); };
}

void SelectionSort::step() {
    if (idx >= data->size()) {
        finished = true;
        return;
    }
    int m = idx;

    for (int i = idx; i < data->size(); i++) {
        if (data->compare(i, m) < 0) {
            m = i;
        }
    }

    data->swap(idx, m);

    idx++;
    stats.steps++;
}

void SelectionSort::_reset() {
    idx = 0;
}
