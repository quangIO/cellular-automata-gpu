#pragma once

#include <arrayfire.h>
#include <parameter.h>
#include <rules.h>


af::array simulate(const Parameter &config, uint order, uint steps = UINT32_MAX);

extern "C" {
    float evaluate(
            float breed_f1,
            float breed_f2,
            float breed_f3,
            float diffusion_f,
            int spread_threshold,
            float spread_f,
            float road_breed_f1);
    void init_simulation();
}
