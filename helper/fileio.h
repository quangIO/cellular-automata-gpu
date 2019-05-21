#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <arrayfire.h>
#include <memory>

namespace IO {
    namespace {
        af::array loadFromPlain(std::string const &path) {
            std::ifstream input;
            static std::string current;
            int size = 0;
            int cnt = 0;
            auto raw = std::make_unique<float[]>(size * size);
            while (input >> current)
                for (auto c: current)
                    raw[cnt++] = c - '0';
            af::array dst(size, size, raw.get());
            return dst;
        }
    }
}
