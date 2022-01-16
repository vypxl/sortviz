#include <iostream>

#include "sorts.hpp"
#include "viz.hpp"


int main() {
    Viz v;
    if (v.init() != 0) {
        std::cerr << "Failed to initialize" << std::endl;
        return -1;
    };
    v.loop();
    return 0;
}
