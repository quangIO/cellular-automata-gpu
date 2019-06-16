using BlackBoxOptim
import Libdl

Libdl.dlopen("/home/quangio/CLionProjects/cellular_automata/cmake-build-debug/libsimulation_lib.so", Libdl.RTLD_GLOBAL)
ccall((:init_simulation, "libsimulation_lib"), Nothing, ())

function f(x)
  1 - Float64(ccall((:evaluate, "libsimulation_lib"), Float32, (Float32, Float32, Float32, Float32, Int32, Float32, Float32), x[1], x[2], x[3], x[4], Int32(1000), x[5], x[6]))
end

res = bboptimize(f; SearchRange = [(.0, 1.0), (.0, 1.0), (.0, 1.0), (.0, 1.0), (.0, 1.0), (.0, 1.0)], MaxTime=1000.0, Method = :xnes)