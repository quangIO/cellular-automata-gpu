#define SIMULATION_SIZE 8

#include "../rules.h"
#include <catch2/catch.hpp>

TEST_CASE("Get size blob", "[Rule][Spread]") {
    af::info();
    int image[64] = {0, 0, 0, 0, 1, 0, 1, 0,
                     0, 0, 0, 0, 0, 0, 1, 1,
                     0, 1, 0, 1, 0, 0, 0, 0,
                     0, 0, 1, 0, 1, 1, 0, 1,
                     1, 1, 0, 0, 0, 1, 0, 1,
                     0, 0, 0, 1, 0, 0, 0, 1,
                     0, 0, 0, 0, 1, 0, 0, 1,
                     0, 1, 0, 0, 0, 1, 0, 0};
    af::array in(8, 8, image);
    auto regions = af::regions(in.as(b8), AF_CONNECTIVITY_8);
    af::print("regions", regions.as(u32));

    af::array ret = af::constant(0, SIMULATION_SIZE, SIMULATION_SIZE);
    for (uint i = 1; i <= 6; ++i) {
        auto current = (regions == i).as(f32);
        ret = ret + current * af::count<float >(current);
    }
    af::print("blob sizes", Rules::Spread::getRegionSizes(regions, 6).as(u32));
    REQUIRE(af::max<int>(regions) == 6);
    REQUIRE(af::allTrue<bool>(ret == Rules::Spread::getRegionSizes(regions, 6)));
}