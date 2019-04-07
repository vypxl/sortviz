#include "sorts.hpp"

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
    delay(10000 / data->size());
}

void SelectionSort::_reset() {
    idx = 0;
}
