#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <functional>
#include "../request.hpp"
#include "../response.hpp"
#include "../methods/methods.hpp"

namespace JETPP
{
    class Route
    {
    public:
        Route(std::string name, JETPP::Methods method, std::function<void(Request &, Response &)> callback);

        std::string getName();
        JETPP::Methods getMethod();
        void execute(Request &request, Response &response);

    private:
        std::string name;
        JETPP::Methods method;
        std::function<void(Request &, Response &)> callback;
    };
}

#endif
