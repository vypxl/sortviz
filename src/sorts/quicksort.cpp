#include "sorts.hpp"
#include <stack>

int QuickSort::partition(int lo, int hi) {
    // int x = data->get(hi);
    int i = lo - 1;

    for (int j = lo; j < hi; j++) {
        // if (data->get(j) <= x) {
        if (data->compare(j, hi) <= 0) {
            i++;
            data->swap(i, j);
        }
    }

    data->swap(i + 1, hi);
    return i + 1;
}

void QuickSort::step() {
    if (locked) return;

    if (stk.size() == 0) {
        finished = true;
        return;
    };
    auto cur = stk.top();
    stk.pop();
    if (cur.first >= cur.second) return;
    
    int p = partition(cur.first, cur.second);
    stk.push(std::pair<int, int>(cur.first, p - 1));
    stk.push(std::pair<int, int>(p + 1, cur.second));

    stats.steps++;
    delay(10000 / data->size());
}

void QuickSort::_reset() {
    locked = true;
    while(stk.size() != 0) stk.pop();
    stk.push(std::pair<int, int>(0, data->size() - 1));
    locked = false;
}

