#include <iostream>
#include <arrayfire.h>
#include "helper/fileio.h"
#include "simulation.h"
#include "rules.h"


int main() {
    af::info();
    af::Window stateWindow(512, 512, "CA Simulation");
    // af::Window spreadWindow(512, 512, "Spread");
    int frame_count = 0;

    af::array grid = (af::randu(SIMULATION_SIZE, SIMULATION_SIZE) <= .000001);
    af::array road;

    Rules::init();

    af::timer::start();
    while (!stateWindow.close()) {
        // af::timer delay = af::timer::start();
        stateWindow.image(grid.as(f32)); // TODO: Pretty visualization
        frame_count++;

        af::array neighbour = Rules::countNeighbours(grid, 0);
        af::array breed = Rules::urbanize(1 - af::pow(Rules::Breed::masks[0], neighbour));
        grid = ((grid + breed) > 0).as(f32);


        af::array region = af::regions(grid.as(b8), AF_CONNECTIVITY_8);
        uint nRegions = af::max<uint>(region);
        af::array regionSize = Rules::getRegionSizes(region, nRegions);
        af::array edge = (af::maxfilt(regionSize) > Rules::Spread::threshold);
        af::array spread = Rules::urbanize(edge * Rules::Spread::factor);
        grid = ((grid + spread) > 0).as(f32);

        // static double fps = 60;
        // while(af::timer::stop(delay) < (1 / fps)) { }
    }
    std::cout << frame_count << " " << frame_count / af::timer::stop();
}
