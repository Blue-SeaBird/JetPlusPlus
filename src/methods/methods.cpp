#include "methods.hpp"
#include <algorithm>
#include <string>
#include <iostream>

namespace JETPP
{
    Methods stringToMethod(std::string method)
    {
        std::transform(method.begin(), method.end(), method.begin(), ::toupper);
        if (method == "GET")
            return JETPP::Methods::Get;
        else if (method == "POST")
            return JETPP::Methods::Post;
        else if (method == "PATCH")
            return JETPP::Methods::Patch;
        else if (method == "DELETE")
            return JETPP::Methods::Delete;
        else if (method == "OPTIONS")
            return JETPP::Methods::Options;
        else
        {
            std::cerr << "Unsupported method: " << method << std::endl;
            throw std::invalid_argument("Invalid method");
        }
    }

}