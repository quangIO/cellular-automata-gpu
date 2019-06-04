#include <iostream>
#include <arrayfire.h>
#include "simulation.h"


int main() {
    // Parameter::Base::a;
    af::info();
    // af::Window stateWindow(512, 512, "CA Simulation");
    // af::Window roadWindow(512, 512, "Road");
    Parameter config("config.toml");
    Rule::init();
    simulate(config, 0);
}
