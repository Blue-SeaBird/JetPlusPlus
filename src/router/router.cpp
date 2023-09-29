#include "router.hpp"

namespace JETPP
{
    Router::Router() {}

    void Router::get(const std::string &routeurl, std::function<void(Request &, Response &)> callback)
    {
        Route route(routeurl, callback);
        this->routes.push_back(route);
    }

    void Router::post(const std::string &routeurl, std::function<void(Request &, Response &)> callback)
    {
        Route route(routeurl, callback);
        this->routes.push_back(route);
    }

    std::vector<Route> Router::getRoutes()
    {
        return this->routes;
    }

    Route Router::findRoute(std::string name)
    {
        std::vector<Route> routes = this->routes;

        for (int i = 0; i < routes.size(); i++)
        {
            if (routes[i].getName() == name)
            {
                return routes[i];
            }
        }
        throw std::runtime_error("Route not found: " + name);
    }

}
