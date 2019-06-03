#include <iostream>
#include <arrayfire.h>
#include "helper/cpptoml.h"
#include "rules.h"
#include "visualizer.h"


int main(int argc, char *argv[]) {
    // Parameter::Base::a;
    af::Window stateWindow(512, 512, "CA Simulation");
    // af::Window roadWindow(512, 512, "Road");
    int frame_count = 0;

    std::string dir = argc > 1 ? argv[1] : ".";

    Rule::init("config.toml");


    auto land = af::loadImage("land.png");
    af::array road = af::loadImage("road.png") > 0;
    Rule::policy = af::loadImage("policy.png") / 255;

    assert(land.dims() == road.dims() && Rule::policy.dims() == land.dims());

    SIMULATION_SIZE = land.dims();

    while (!stateWindow.close()) {
        // af::timer delay = af::timer::start();
        // af::array display = af::join(2, road.as(f32), land.as(f32), road.as(f32));
        // stateWindow.image(display.as(f32));
        frame_count++;

        {
            af::array diffusion = Rule::urbanize(Parameter::Land::Diffusion::factor);
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
            uint numberOfRegion = af::max<uint>(region);

            af::array hist = af::histogram(region, numberOfRegion);
            auto feasibleRegions = af::where(hist > Parameter::Land::Spread::threshold);
            auto feasibleRegionsHost = feasibleRegions.host<uint>();

            af::array mask = region > 0;
            for (size_t i = 0; i < feasibleRegions.dims(0); ++i)
                mask = mask & (feasibleRegionsHost[i] != region);
            float probability = Parameter::Land::Spread::factor;
            af::array edge = probability * af::maxfilt(mask);
            af::array spread = Rule::urbanize(edge);
            land = (land + spread) > 0;
            af::freeHost(feasibleRegionsHost);
        }
        {
            static std::vector<af::array> breedMasks = Rule::Breed::generate(Parameter::Road::Breed::factors);
            af::array neighbour = Rule::countNeighbours(road, 0);
            af::array breed = Rule::urbanize(1 - af::pow(breedMasks[0], neighbour));
            land = (land + breed) > 0;
        }
        if (frame_count % 10 == 0)
            std::cout << frame_count << std::endl;
        // static double fps = 60;
        // while (af::timer::stop(delay) < (1 / fps)) {}
    }
    std::cout << frame_count;
}
