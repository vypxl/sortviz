#pragma once

#include <stack>

#include "sort.hpp"
#include "arraywrapper.hpp"

class SelectionSort : public Sort {
private:
    int idx = 0;
protected:
    void step();
    void _reset();
public:
    SelectionSort(ArrayWrapper *ary) : Sort(ary) {};
    void dispose();
};

class BubbleSort : public Sort {
private:
    int n = 0;
    bool swapped = false;
protected:
    void step();
    void _reset();
public:
    BubbleSort(ArrayWrapper *ary) : Sort(ary) {};
    void dispose();
};

class QuickSort : public Sort {
private:
    std::stack<std::pair<int, int>> stk;
    bool locked = false;

    int partition(int lo, int hi);
protected:
    void step();
    void _reset();
public:
    QuickSort(ArrayWrapper *ary) : Sort(ary) {};
    void dispose();
};
