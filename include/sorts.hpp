#pragma once
#include <stack>
// #include <pair>
#include "sort.hpp"

class SelectionSort : public Sort {
private:
    int idx = 0;
protected:
    void step();
public:
    void reset();
};

class BubbleSort : public Sort {
private:
    int n = 0;
    bool swapped = false;
protected:
    void step();
public:
    void reset();
};

class QuickSort : public Sort {
private:
    std::stack<std::pair<int, int>> stk;
    bool locked = false;

    int partition(int lo, int hi);
protected:
    void step();
public:
    void reset();
};
