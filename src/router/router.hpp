#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "route.hpp"
#include <string>
#include <vector>
#include <functional>
#include "../request.hpp"
#include "../response.hpp"

namespace JETPP
{
    class Router
    {
    public:
        Router();

        void get(const std::string &routeurl, std::function<void(Request &, Response &)> callback);
        void post(const std::string &routeurl, std::function<void(Request &, Response &)> callback);
        std::vector<Route> getRoutes();
        Route findRoute(std::string name);

    private:
        std::vector<Route> routes;
    };
}

#endif
