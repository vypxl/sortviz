#ifndef __sorts_hpp_
#define __sorts_hpp_

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
    SelectionSort(ArrayWrapper *ary);
};

class BubbleSort : public Sort {
private:
    int n = 0;
    bool swapped = false;
protected:
    void step();
    void _reset();
public:
    BubbleSort(ArrayWrapper *ary);
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
    QuickSort(ArrayWrapper *ary);
};

class InsertionSort : public Sort {
private:
  int idx = 0;
protected:
  void step();
  void _reset();
public:
  InsertionSort(ArrayWrapper *ary);
};


/* Add new Sort definition here*/


namespace Sorts {
    enum Sorts {
        SelectionSort,
        BubbleSort,
        InsertionSort,
        QuickSort,

        /* Register new sort here */

        /* DO NOT TOUCH __count_, it must be the last element in this enum! */
        __count_
    };

    const std::string names[4] {
        "Selection Sort",
        "Bubble Sort",
        "Insertion Sort",
        "Quick Sort",

        /* Set new sort display name here */
    };

    Sort* create(Sorts which, ArrayWrapper *data);
}

#endif
