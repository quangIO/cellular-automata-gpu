#include <arrayfire.h>
#include <vector>
#include <memory>
#include <cassert>
#define SIMULATION_SIZE 1000

namespace State {
    static af::array grid(SIMULATION_SIZE, SIMULATION_SIZE); // NOLINT(cert-err58-cpp)
}


namespace Rules {
    namespace Masks {
        namespace {
            std::vector<af::array> spreadMasks;
            void generate(const std::vector<float> &factors) {
                for (float factor: factors){
                    assert(factor > 0 && factor < 1);
                    spreadMasks.emplace_back(af::constant(1 - factor, SIMULATION_SIZE, SIMULATION_SIZE));
                }
            }
        }
    }
    namespace Kernels {
        namespace {
            std::vector<af::array> neighboursCountKernels;

            void generate(unsigned nLayers = 1) {
                for (size_t i = 1; i <= nLayers; ++i) {
                    size_t m = 1u << i | 1u;
                    m *= m;
                    auto arr = std::make_unique<float[]>(m);
                    for (size_t j = 0; j < m; ++j)
                        arr[j] = 1.0f;
                    arr[m >> 1u] = 0.0;
                    int size = (1u << i) | 1u;
                    const af::array t(size, size, arr.get());
                    neighboursCountKernels.emplace_back(t);
                }
            }
        }
    }

    static af::array countNeighbours(unsigned offset = 0, const af::array &grid = State::grid) {
        af::array neighbours = af::convolve(grid, Kernels::neighboursCountKernels[offset]);
        return neighbours;
    }

    static void init() {
        Kernels::generate();
        Masks::generate({.01});
    }

    void setSpreadFactor(float value) {
    }
};