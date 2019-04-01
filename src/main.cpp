#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <thread>

#include "config.hpp"
#include "sort.hpp"
#include "sorts.hpp"
#include "arraywrapper.hpp"

#include "viz.hpp"


int main() {
    ArrayWrapper ary(1000);
    QuickSort sort(&ary);
    Viz v;
    if (v.init() != 0) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    };
    v.changeStyle(Viz::Styles::Colorloop);
    v.setSort(&sort);
    v.loop();
    sort.stop();
    return 0;
}
