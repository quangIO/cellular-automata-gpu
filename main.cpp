#include <iostream>
#include <arrayfire.h>
#include "helper/cpptoml.h"
#include "rules.h"
#include "visualizer.h"

#define ACCURATE

int main(int argc, char *argv[]) {
    // Parameter::Base::a;
    af::Window stateWindow(512, 512, "CA Simulation");
    // af::Window roadWindow(512, 512, "Road");
    int frame_count = 0;

    std::string dir = argc > 1 ? argv[1] : ".";

    Rule::init("config.toml");


    af::array land = af::loadImage("land.png") > 0;
    af::array road = af::loadImage("road.png") > 0;
    Rule::policy = af::loadImage("policy.png") / 255;

    assert(land.dims() == road.dims() && Rule::policy.dims() == land.dims());

    SIMULATION_SIZE = land.dims();

    while (!stateWindow.close()) {
        af::timer delay = af::timer::start();
        af::array display = af::join(2, road.as(f32), land.as(f32), road.as(f32));
        stateWindow.image(display.as(f32)); // TODO: Pretty visualization
        frame_count++;

        {
            af::array diffusion = Rule::urbanize(af::constant(Parameter::Land::Diffusion::factor, SIMULATION_SIZE));
            land = (land + diffusion) > 0;
        }

        {
            static std::vector<af::array> breedMasks = Rule::Breed::generate(Parameter::Land::Breed::factors);
            af::array neighbour = Rule::countNeighbours(land, 0);
            af::array breed = Rule::urbanize(1 - af::pow(breedMasks[0], neighbour));
            land = (land + breed) > 0;
        }

        {
            af::array region = af::regions(land.as(b8), AF_CONNECTIVITY_8);
            uint nRegions = af::max<uint>(region);
#ifndef ACCURATE
            af::array regionSize = Rule::getRegionSizes(region, nRegions);
            af::array edgeMask = (af::maxfilt(regionSize) > Rule::Spread::threshold);
            af::array spread = Rule::urbanize(edgeMask * Rule::Spread::factor);
            land = ((land + spread) > 0).as(f32);
#else
            for (int i = 1; i <= nRegions; ++i) {
                af::array regionMask = region == i;
                uint regionSize = af::count<uint>(regionMask);
                float probability = Rule::Spread::calculateProbability(regionSize, Parameter::Land::Spread::threshold,
                                                                       Parameter::Land::Spread::factor);
                af::array edge = probability * af::maxfilt(regionMask);
                af::array spread = Rule::urbanize(edge);
                land = (land + spread) > 0;
            }
#endif
        }

        {
            static std::vector<af::array> breedMasks = Rule::Breed::generate(Parameter::Road::Breed::factors);
            af::array neighbour = Rule::countNeighbours(road, 0);
            af::array breed = Rule::urbanize(1 - af::pow(breedMasks[0], neighbour));
            land = (land + breed) > 0;
        }
        static double fps = 60;
        while (af::timer::stop(delay) < (1 / fps)) {}
    }
    std::cout << frame_count;
}
