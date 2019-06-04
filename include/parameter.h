#pragma once
#include "helper/cpptoml.h"

static int YEARS = 1;

struct Parameter {
    struct Land {
        std::vector<float> breed_factors = {0.01f};
        float diffusion_factor = 0;
        uint spread_threshold = 100;
        float spread_factor = .003f;
    } land;

    struct Road {
        std::vector<float> breed_factors = {0.02f};
    } road;

    explicit Parameter(const std::string &path) {
        auto root = cpptoml::parse_file(path);
        YEARS = root->get_as<int>("year").value_or(YEARS);
        {
            auto table = root->get_table("land");
            auto breed = table->get_table("breed");
            auto temp = *breed->get_array_of<double>("factors");
            land.breed_factors.resize(temp.size());
            for (uint i = 0; i < temp.size(); ++i) land.breed_factors[i] = temp[i];
            auto spread = table->get_table("spread");
            land.spread_threshold = spread->get_as<int>("threshold").value_or(land.spread_threshold);
            land.spread_factor = *spread->get_as<double>("factor");
            auto diffusion = table->get_table("diffusion");
            land.diffusion_factor = diffusion->get_as<double>("factor").value_or(land.diffusion_factor);
        }
        {
            auto table = root->get_table("road");
            auto breed = table->get_table("breed");
            auto temp = *breed->get_array_of<double>("factors");
            road.breed_factors.resize(temp.size());
            for (uint i = 0; i < temp.size(); ++i) road.breed_factors[i] = temp[i];
        }
    }
};