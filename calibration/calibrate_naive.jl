import Libdl

Libdl.dlopen("/home/quangio/CLionProjects/cellular_automata/cmake-build-debug/libsimulation_lib.so", Libdl.RTLD_GLOBAL)
ccall((:init_simulation, "libsimulation_lib"), Nothing, ())


function simulate(x)
  1 - Float64(ccall((:evaluate, "libsimulation_lib"), Float32, (Float32, Float32, Float32, Float32, Int32, Float32, Float32), x[1], x[2], x[3], x[4], Int32(x[7]), x[5], x[6]))
end

breed_f1_step() = .1

let
  min_score = 1.0
  best_params = []

  for breed_f1 = 0.0:breed_f1_step():1.0 # TODO: Use adaptive steps
    for breed_f2 = 0.0:breed_f1_step():1.0
      for breed_f3 = 0.0:breed_f1_step():1.0
        for diffusion_f = 0.0:breed_f1_step():1.0
          for spread_f = 0.0:breed_f1_step():1.0
            for road_breed_f1 = 0.0:breed_f1_step():1.0
              params = [breed_f1, breed_f2, breed_f3, diffusion_f, spread_f, road_breed_f1, 1000]
              score = simulate(params)
              if score < min_score
                best_params = params
                min_score = score
                print(min_score)
                print(" ")
                println(best_params)
              end
            end
          end
        end
      end
    end
  end
end

"ok"
