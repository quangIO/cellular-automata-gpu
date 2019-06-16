#pragma once

#include <arrayfire.h>
#include <vector>
#include <memory>
#include <cassert>
#include <parameter.h>

struct Input {
    inline static std::vector<af::array> policies;
    inline static std::vector<af::array> lands;
    inline static std::vector<af::array> roads;
    inline static af::dim4 SIMULATION_SIZE;
    static void init();
};

namespace Rule {

    namespace Breed {
        static std::vector<af::array> generate(const std::vector<float> &factors) {
            std::vector<af::array> masks;
            masks.reserve(factors.size());
            for (float factor: factors) {
                assert(factor >= 0 && factor <= 1);
                masks.emplace_back(af::constant(1 - factor, Input::SIMULATION_SIZE));
            }
            return masks;
        }
    }

    namespace Spread {
        inline float calculateProbability(uint regionSize, uint threshold, float factor) {
            return (regionSize >= threshold) * factor;
        }
    }

    struct Kernels {
        inline static std::vector<af::array> neighboursCountKernels;

        static void generate(uint nLayers) {
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
    };


    namespace {
        // af::array weight = af::constant(1, SIMULATION_SIZE);
        inline af::array countNeighbours(const af::array &grid, uint offset = 0) {
            return af::convolve(grid, Kernels::neighboursCountKernels[offset]);
        }

        template <typename T>
        inline af::array urbanize(const T &probabilities, uint year = 0) {
            return af::randu(Input::SIMULATION_SIZE) < probabilities * Input::policies[year];
        }

        void init() {
            Input::init();
            Kernels::generate(10);
        }
    }
}