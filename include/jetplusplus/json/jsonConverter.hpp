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
            std::string jsonToString(JETPP::JsonValue value);
            JETPP::JsonValue stringToJson(std::string value);
            void splitString(std::string str, std::vector<std::string> &segments, char delimiter);
        private:
    };
} // namespace JETPP

#endif
