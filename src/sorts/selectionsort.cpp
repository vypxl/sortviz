#include "sorts.hpp"

void SelectionSort::step() {
    if (idx >= data->size()) return;
    int m = idx;
    
    for (int i = idx; i < data->size(); i++) {
        if (data->compare(i, m) < 0) {
            m = i;
        }
    }

    data->swap(idx, m);

    idx++;
}

void SelectionSort::reset() {
    idx = 0;
}

