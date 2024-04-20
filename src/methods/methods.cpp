#include "../../include/jetplusplus/methods/methods.hpp"
#include <algorithm>
#include <string>
#include <iostream>

namespace jetpp
{
    Methods stringToMethod(std::string method)
    {
        std::transform(method.begin(), method.end(), method.begin(), ::toupper);
        if (method == "GET")
            return jetpp::Methods::Get;
        else if (method == "POST")
            return jetpp::Methods::Post;
        else if(method == "PUT")
            return jetpp::Methods::Put;
        else if (method == "PATCH")
            return jetpp::Methods::Patch;
        else if (method == "DELETE")
            return jetpp::Methods::Delete;
        else if (method == "OPTIONS")
            return jetpp::Methods::Options;
        else
        {
            std::cerr << "Unsupported method: " << method << std::endl;
            throw std::invalid_argument("Invalid HTTP method");
        }
    }

}