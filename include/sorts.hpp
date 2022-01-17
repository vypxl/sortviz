#ifndef __sorts_hpp_
#define __sorts_hpp_

#include <iostream>
#include <stack>

#include "sort.hpp"
#include "arraywrapper.hpp"

//////////////////////////////////////////////////////////
// Look for /**/ to find where to insert extension code //
//////////////////////////////////////////////////////////

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


/* Add new Sort definition here*/


namespace Sorts {
    enum Sorts {
        SelectionSort,
        BubbleSort,
        QuickSort,

        /* Register new sort here */

        /* DO NOT TOUCH __count_, it must be the last element in this enum! */
        __count_
    };

    const std::string names[3] {
        "Selection Sort",
        "Bubble Sort",
        "Quick Sort",

        /* Set new sort display name here */
    };

    Sort* create(Sorts which, ArrayWrapper *data);
}

#endif
