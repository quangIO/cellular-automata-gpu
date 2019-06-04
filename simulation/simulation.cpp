#include <simulation.h>
#include <iostream>

af::array simulate(const Parameter &config, uint order, uint steps) {
    af::array land = Input::lands[order];
    af::array road = Input::roads[order];
    af::array policy = Input::policies[order];

    int frame_count = 0;

    while (frame_count++ < steps) {
        // af::timer delay = af::timer::start();
        // af::array display = af::join(2, road.as(f32), land.as(f32), road.as(f32));
        // stateWindow.image(display.as(f32));
        {
            af::array diffusion = Rule::urbanize(config.land.diffusion_factor);
            land = (land + diffusion) > 0;
        }
        {
            static std::vector<af::array> breedMasks = Rule::Breed::generate(config.land.breed_factors);
            af::array neighbour = Rule::countNeighbours(land, 0);
            af::array breed = Rule::urbanize(1 - af::pow(breedMasks[0], neighbour));
            land = (land + breed) > 0;
        }
        {
            af::array region = af::regions(land.as(b8), AF_CONNECTIVITY_8);
            uint numberOfRegion = af::max<uint>(region);

            af::array hist = af::histogram(region, numberOfRegion);
            auto feasibleRegions = af::where(hist > config.land.spread_threshold);
            auto feasibleRegionsHost = feasibleRegions.host<uint>();

            af::array mask = region > 0;

            for (uint i = 0; i < feasibleRegions.elements(); ++i)
                mask = mask & (feasibleRegionsHost[i] != region);
            float probability = config.land.spread_factor;
            af::array edge = probability * af::maxfilt(mask);
            af::array spread = Rule::urbanize(edge);
            land = (land + spread) > 0;
            af::freeHost(feasibleRegionsHost);
        }
        {
            static std::vector<af::array> breedMasks = Rule::Breed::generate(config.road.breed_factors);
            af::array neighbour = Rule::countNeighbours(road, 0);
            af::array breed = Rule::urbanize(1 - af::pow(breedMasks[0], neighbour));
            land = (land + breed) > 0;
        }
        if (frame_count % 10 == 0)
            std::cout << frame_count << std::endl;
        // static double fps = 60;
        // while (af::timer::stop(delay) < (1 / fps)) {}
    }
    return land;
}