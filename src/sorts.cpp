#include "arraywrapper.hpp"
#include "sort.hpp"
#include "sorts.hpp"

#include <iostream>

Sort* Sorts::create(Sorts which, ArrayWrapper *data) {
    #define CASE(X) case X: return new ::X(data); break;
    switch (which)
    {
        CASE(SelectionSort)
        CASE(BubbleSort)
        CASE(InsertionSort)
        CASE(QuickSort)
        /* Also register new sort here */
    
        default: break;
    }
    #undef CASE

    // not reached
    std::cerr << "Unregistered sort " << which << std::endl;
    return new ::SelectionSort(data);
}
