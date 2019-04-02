#include <iostream>

#include "sorts.hpp"
#include "viz.hpp"


int main() {
    Viz v;
    if (v.init() != 0) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    };
    v.changeStyle(Viz::Styles::Colorloop);
    v.setSort<QuickSort>();
    v.loop();
    return 0;
}

// Nextup, define sizes, styles and sorts and create keys to cycle through them
