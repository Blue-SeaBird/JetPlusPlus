#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "../router/route.hpp"
#include <string>
#include <vector>
#include <optional>
#include "../server/request.hpp"
#include "../server/response.hpp"
#include "../container/container.hpp"

namespace JETPP
{
    class Router
    {
    public:
        Router();
        
        //with default container
        void get(const std::string &routeurl, void (*callback)(Request &, Response &));
        void post(const std::string &routeurl, void (*callback)(Request &, Response &));
        void options(const std::string &routeurl, void (*callback)(Request &, Response &));

        //with container
        void get(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        void post(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        void options(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        std::vector<Route> getRoutes();
        std::optional<Route> findRoute(std::string request, JETPP::Methods method, std::string clientAddress);
        Container &checkContainer(Container &container);

    private:
        std::vector<Route> routes;
        std::vector<Container> container;
        void splitRoute(std::string str, std::vector<std::string> &segments, char delimiter);
        bool checkAccess(Container container, std::string clientAddress);
    };
}

#endif
