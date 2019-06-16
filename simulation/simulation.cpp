#include <simulation.h>
#include <iostream>

af::array simulate(const Parameter &config, uint order, uint steps) {
    af::array land = Input::lands[order].copy();
    af::array road = Input::roads[order].copy();
    af::array policy = Input::policies[order].copy();

    int frame_count = 0;
    // af::Window stateWindow(512, 512, "CA Simulation");
    while (frame_count++ < steps) {
        // af::timer delay = af::timer::start();
        // af::array display = af::join(2, road.as(f32), land.as(f32), road.as(f32));
        // stateWindow.image(land.as(f32));
        {
            af::array diffusion = Rule::urbanize(config.land.diffusion_factor);
            land = (land + diffusion) > 0;
        }
        for (uint i = 0; i < config.land.breed_factors.size(); ++i) {
            static std::vector<af::array> breedMasks = Rule::Breed::generate(config.land.breed_factors);
            af::array neighbour = Rule::countNeighbours(land, i);
            af::array breed = Rule::urbanize(1 - af::pow(breedMasks[i], neighbour));
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
            const float probability = config.land.spread_factor;
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
        // static double fps = 60;
        // while (af::timer::stop(delay) < (1 / fps)) {}
    }
    af::saveImage("out.png", land.as(f32));
    return land;
}

const int SIMULATION_ITERATION = 1;

float evaluate(float breed_f1, float breed_f2, float breed_f3, float diffusion_f, int spread_threshold,
               float spread_f,
               float road_breed_f1) {
    std::vector land_breed{breed_f1, breed_f2, breed_f3};
    Parameter parameter;
    parameter.land.breed_factors = land_breed;
    parameter.land.diffusion_factor = diffusion_f;
    parameter.land.spread_threshold = spread_threshold;
    parameter.land.spread_factor = spread_f;
    parameter.road.breed_factors = {road_breed_f1};

    float score = 0;
    af::array result = simulate(parameter, 0, 10);


    for (uint iteration = 0; iteration < SIMULATION_ITERATION; ++iteration) {
        for (uint i = 1; i < 2; ++i) {
            score += 1.0 * af::count<int>(result & Input::lands[i]) / af::count<int>(result | Input::lands[i]);
        }
    }

    if (score / SIMULATION_ITERATION > .6) std::cout << "nice" << std::endl;
    return score / SIMULATION_ITERATION;
}

void init_simulation() {
    af::info();
    Rule::init();
    std::cout << "Initialized" << std::endl;
}
