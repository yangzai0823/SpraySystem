#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H
#include "Include/Core/Config/config.hpp"
#include "Include/Core/visibility_control.hpp"
#include <string>
namespace vws {
    namespace properties {
        using Config = vws::properties::Config;
        class CORE_MODULE JsonSerializer {
        public:
            static int fromJson(Config& config, std::string filename);
            static int toJson(const Config& config, const std::string filename);
        };
    }
}

#endif
