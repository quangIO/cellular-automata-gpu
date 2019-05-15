#pragma once
#include <arrayfire.h>
#include <vector>
#include <memory>
#include <cassert>

#ifndef SIMULATION_SIZE
#define SIMULATION_SIZE  1000
#endif

typedef unsigned uint;


namespace Rules {
    namespace Diffusion {
        namespace {
            af::array mask;

            void generate(float *probability) {
                mask = af::array(SIMULATION_SIZE, SIMULATION_SIZE, probability);
            }
        }
    }
    namespace Breed {
        namespace {
            std::vector<af::array> masks;

            void generate(const std::vector<float> &factors) {
                for (float factor: factors) {
                    assert(factor > 0 && factor < 1);
                    masks.emplace_back(af::constant(1 - factor, SIMULATION_SIZE, SIMULATION_SIZE));
                }
            }
        }
    }

    namespace Spread {
        namespace {
            int threshold = 100;
            float factor = .003;
        }
    }

    namespace Kernels {
        namespace {
            std::vector<af::array> neighboursCountKernels;

            void generate(uint nLayers = 1) {
                for (uint i = 1; i <= nLayers; ++i) {
                    uint m = 1u << i | 1u;
                    m *= m;
                    auto arr = std::make_unique<float[]>(m);
                    for (uint j = 0; j < m; ++j)
                        arr[j] = 1.0f;
                    arr[m >> 1u] = 0.0;
                    uint size = (1u << i) | 1u;
                    const af::array t(size, size, arr.get());
                    neighboursCountKernels.emplace_back(t);
                }
            }
        }
    }


    namespace {
        inline af::array countNeighbours(const af::array &grid, uint offset = 0) {
            af::array neighbours = af::convolve(grid, Kernels::neighboursCountKernels[offset]);
            return neighbours;
        }

        inline af::array getRegionSizes(const af::array &regions, uint nRegions) {
            af::array ret = af::constant(0, SIMULATION_SIZE, SIMULATION_SIZE);
            for (uint i = 1; i <= nRegions; ++i) {
                auto current = (regions == i).as(f32);
                ret = ret + current * af::count<float>(current);
            }
            return ret;
        }

        inline af::array urbanize(const af::array &probabilities) {
            return af::randu(SIMULATION_SIZE, SIMULATION_SIZE) <= probabilities;
        }

        void initWith(const std::string &path = "config.ini") {
            INIReader reader(path);
            {
                std::string factorsStr = reader.Get("breed", "factors", "0.01");
                std::stringstream s(factorsStr);
                std::vector<float> factors;
                float current;
                while (s >> current) factors.push_back(current);
                Breed::generate(factors);
            }
            {
                Spread::threshold = reader.GetInteger("spread", "threshold", Spread::threshold);
                Spread::factor = reader.GetReal("spread", "factor", Spread::factor);
            }
            {
                std::string imagePath = reader.Get("initial", "image", "state.png"); // TODO load initial image
            }
        }

        void init() {
            Kernels::generate();
            Breed::generate({.01});
        }
    }
}
