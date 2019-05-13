#include <iostream>
#include <arrayfire.h>
#include "simulation.h"
#include "rules.h"


int main() {
    af::info();
    // af::Window stateWindow(512, 512, "CA Simulation");
    // af::Window spreadWindow(512, 512, "Spread");
    int frame_count = 0;

    State::grid = (af::randu(SIMULATION_SIZE, SIMULATION_SIZE) <= .000001).as(f32);

    Rules::init();

    af::timer::start();
    while(frame_count < 3000) {
        // af::timer delay = af::timer::start();
        // stateWindow.image(State::grid);
        frame_count++;

        {
            af::array neighbours = Rules::countNeighbours(0);
            af::array breed = Rules::urbanize(1 - af::pow(Rules::Breed::masks[0], neighbours));
            State::grid = ((State::grid + breed) > 0).as(f32);
        }

        {
            af::array regions = af::regions(State::grid.as(b8), AF_CONNECTIVITY_8);
            int nRegions = af::max<int>(regions);
            af::array regionSize = Rules::Spread::getRegionSizes(regions, nRegions);
            af::array edges = (af::maxfilt(regionSize) > Rules::Spread::threshold);
            af::array spread = Rules::urbanize(edges * Rules::Spread::factor);
            State::grid = ((State::grid + spread) > 0).as(f32);
        }

        // static double fps = 60;
        // while(af::timer::stop(delay) < (1 / fps)) { }
    }
    std::cout << frame_count << " " << frame_count / af::timer::stop();
}
