#include <iostream>
#include <arrayfire.h>
#include "simulation.h"


int main() {
    // Parameter::Base::a;
    af::info();
    // af::Window stateWindow(512, 512, "CA Simulation");
    // af::Window roadWindow(512, 512, "Road");
    Rule::init();
    evaluate(0.001, 0, 0, 0, 1000, 0, 0.002);
}
