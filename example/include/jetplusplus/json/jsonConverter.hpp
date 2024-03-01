#ifndef JSONCONVERTER_HPP
#define JSONCONVERTER_HPP
#include <string>
#include <vector>
#include "value.hpp"
#include <sstream>

namespace JETPP
{
    class JsonConverter{
        public:
            JsonConverter();
            std::string jsonToString(JETPP::JsonValue value) const;
            JETPP::JsonValue stringToJson(std::string value) const;
            void splitString(std::string str, std::vector<std::string> &segments, char delimiter) const;
    };
} // namespace JETPP

#endif
