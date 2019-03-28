#include "sort.hpp"
#include "sorts.hpp"
#include "config.hpp"
#include <stack>

int QuickSort::partition(int lo, int hi) {
    int x = data[hi];
    int i = lo - 1;

    for (int j = lo; j < hi; j++) {
        if (data[j] <= x) {
            i++;
            swap(i, j);
        }
    }

    swap(i + 1, hi);
    return i + 1;
}

void QuickSort::step() {
    if (locked) return;

    if (stk.size() == 0) return;
    auto cur = stk.top();
    stk.pop();
    if (cur.first >= cur.second) return;
    
    int p = partition(cur.first, cur.second);
    stk.push(std::pair<int, int>(cur.first, p - 1));
    stk.push(std::pair<int, int>(p + 1, cur.second));

    delay();
}

void QuickSort::reset() {
    locked = true;
    while(stk.size() != 0) stk.pop();
    stk.push(std::pair<int, int>(0, SIZE - 1));

    shuffle();
    locked = false;
}
