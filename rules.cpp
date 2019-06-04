#include <rules.h>


void Input::init() {
    af::array initial = af::loadImage("land.0.png");
    SIMULATION_SIZE = initial.dims();
    for (uint i = 0; i < YEARS; ++i) {
        lands.emplace_back(af::loadImage(("land." + std::to_string(i) + ".png").c_str()) > 0);
        roads.emplace_back(af::loadImage(("road." + std::to_string(i) + ".png").c_str()) > 0);
        policies.emplace_back(af::loadImage(("policy." + std::to_string(i) + ".png").c_str()) / 255);
    }
}
