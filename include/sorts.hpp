#pragma once

#include <stack>

#include "sort.hpp"
#include "arraywrapper.hpp"

class SelectionSort : public Sort {
private:
    int idx = 0;
protected:
    void step();
public:
    SelectionSort(ArrayWrapper *ary) : Sort(ary) {};
    void reset();
    void dispose();
};

class BubbleSort : public Sort {
private:
    int n = 0;
    bool swapped = false;
protected:
    void step();
public:
    BubbleSort(ArrayWrapper *ary) : Sort(ary) {};
    void reset();
    void dispose();
};

class QuickSort : public Sort {
private:
    std::stack<std::pair<int, int>> stk;
    bool locked = false;

    int partition(int lo, int hi);
protected:
    void step();
public:
    QuickSort(ArrayWrapper *ary) : Sort(ary) {};
    void reset();
    void dispose();
};
