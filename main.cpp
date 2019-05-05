#include <iostream>
#include <arrayfire.h>
#include "simulation.h"
#include "rules.h"


void breed(af::array& grid, af::array& tmp) {
}

int main() {
    af::info();
    af::Window stateWindow(512, 512, "CA Simulation");
    af::Window spreadWindow(512, 512, "Spread");
    int frame_count = 0;

    State::grid = (af::randu(SIMULATION_SIZE, SIMULATION_SIZE) > .999).as(f32);

    Rules::init();

    while(!stateWindow.close()) {
        af::timer delay = af::timer::start();
        stateWindow.image(State::grid);
        frame_count++;
        af::array neighbours = Rules::countNeighbours(0);
        af::array b = af::randu(SIMULATION_SIZE, SIMULATION_SIZE) > af::pow(Rules::Masks::spreadMasks[0], neighbours);

        State::grid = ((State::grid + b) > 0).as(f32);

        static double fps = 60;
        while(af::timer::stop(delay) < (1 / fps)) { }
    }
}
