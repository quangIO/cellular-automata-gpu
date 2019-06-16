#include <rules.h>
#include <iostream>

namespace Input {
    std::vector<af::array> policies;
    std::vector<af::array> lands;
    std::vector<af::array> roads;
    af::dim4 SIMULATION_SIZE;
};

void Input::init() {
    af::array initial = af::loadImage("land.0.png");
    std::cout << "Loading images of size " << initial.dims() << std::endl;
    SIMULATION_SIZE = initial.dims();

    for (uint i = 0; i < YEARS; ++i) {
        lands.push_back(af::loadImage(("land." + std::to_string(i) + ".png").c_str()) > 0);
        roads.push_back(af::loadImage(("road." + std::to_string(i) + ".png").c_str()) > 0);
        policies.push_back(af::loadImage(("policy." + std::to_string(i) + ".png").c_str()) / 255);
    }

    std::cout << "Scores between images:" << std::endl;
    for (uint i = 1; i < YEARS; ++i) {
        std::cout << 1.0 * af::count<int>(Input::lands[i - 1] & Input::lands[i]) /
                     af::count<int>(Input::lands[i - 1] | Input::lands[i]) << " ";
    }
    std::cout << std::endl;
}

void Input::clear() {
    lands.clear();
    roads.clear();
    policies.clear();
}
