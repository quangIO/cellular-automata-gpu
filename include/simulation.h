#pragma once

#include <arrayfire.h>
#include <parameter.h>
#include <rules.h>


af::array simulate(const Parameter &config, uint order, uint steps = UINT32_MAX);
