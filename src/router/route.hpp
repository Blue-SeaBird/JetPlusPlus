#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <functional>
#include "../request.hpp"
#include "../response.hpp"

namespace JETPP
{
    class Route
    {
    public:
        Route(std::string name, std::function<void(Request &, Response &)> callback);

        std::string getName();
        void execute(Request &request, Response &response);

    private:
        std::string name;
        std::function<void(Request &, Response &)> callback;
    };
}

#endif
