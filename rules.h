#pragma once

#include <arrayfire.h>
#include <vector>
#include <memory>
#include <cassert>
#include "parameter.h"

typedef unsigned uint;

static af::dim4 SIMULATION_SIZE(100, 100);

namespace Rule {
    af::array policy;
    namespace Breed {
        std::vector<af::array> generate(const std::vector<float> &factors) {
            std::vector<af::array> masks;
            masks.reserve(factors.size());
            for (float factor: factors) {
                assert(factor > 0 && factor < 1);
                masks.emplace_back(af::constant(1 - factor, SIMULATION_SIZE));
            }
            return masks;
        }
    }

    namespace Spread {
        inline float calculateProbability(uint regionSize, uint threshold, float factor) {
            return (regionSize >= threshold) * factor;
        }
    }

    namespace Kernels {
        std::vector<af::array> neighboursCountKernels;

        void generate(uint nLayers) {
            neighboursCountKernels.clear();
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


    namespace {
        // af::array weight = af::constant(1, SIMULATION_SIZE);
        inline af::array countNeighbours(const af::array &grid, uint offset = 0) {
            return af::convolve(grid, Kernels::neighboursCountKernels[offset]);
        }

        inline af::array getRegionSizes(const af::array &regions, uint nRegions) {
            af::array ret = af::constant(0, SIMULATION_SIZE);
            for (uint i = 1; i <= nRegions; ++i) {
                auto current = (regions == i).as(f32);
                ret = ret + current * af::count<float>(current);
            }
            return ret;
        }

        inline af::array urbanize(const af::array &probabilities) {
            return af::randu(SIMULATION_SIZE) < probabilities * policy;
        }

        void init(const std::string &path) {
            Parameter::init(path);
            Kernels::generate(10);
        }
    }
}