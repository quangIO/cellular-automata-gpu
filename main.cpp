#include <iostream>
#include <arrayfire.h>
#include "simulation.h"


int main() {
    // Parameter::Base::a;
    // af::Window stateWindow(512, 512, "CA Simulation");
    // af::Window roadWindow(512, 512, "Road");
    init_simulation();
    std::cout << evaluate(0.001, 0, 0, 0, 1000, 0, 0.002);
    Input::clear();
    int a; std::cin >> a;
}
