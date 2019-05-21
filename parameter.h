#include "helper/cpptoml.h"

namespace Parameter {
    namespace Land {
        namespace Breed {
            std::vector<float> factors = {0.01f};
        }
        namespace Diffusion {
            float factor = 0;
        }
        namespace Spread {
            unsigned threshold = 100;
            float factor = .003f;
        }
    }
    namespace Road {
        namespace Breed {
            std::vector<float> factors = {0.02f};
        }
        namespace {}
    }
    namespace {
        void init(const std::string &path) {
            af::info();
            auto root = cpptoml::parse_file(path);
            {
                using namespace Parameter::Land;
                auto land = root->get_table("land");
                auto breed = land->get_table("breed");
                auto temp = *breed->get_array_of<double>("factors");
                Breed::factors.resize(temp.size());
                for (int i = 0; i < temp.size(); ++i) Breed::factors[i] = temp[i];
                auto spread = land->get_table("spread");
                Spread::threshold = spread->get_as<int>("threshold").value_or(Spread::threshold);
                Spread::factor = *spread->get_as<double>("factor");
                auto diffusion = land->get_table("diffusion");
                Diffusion::factor = diffusion->get_as<double>("factor").value_or(Diffusion::factor);
            }
            {
                using namespace Parameter::Road;
                auto road = root->get_table("road");
                auto breed = road->get_table("breed");
                auto temp = *breed->get_array_of<double>("factors");
                Breed::factors.resize(temp.size());
                for (int i = 0; i < temp.size(); ++i) Breed::factors[i] = temp[i];
            }
        }
    }
}