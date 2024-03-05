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
        void put(const std::string &routeurl, void (*callback)(Request &, Response &));
        void patch(const std::string &routeurl, void (*callback)(Request &, Response &));
        void Delete(const std::string &routeurl, void (*callback)(Request &, Response &));
        void options(const std::string &routeurl, void (*callback)(Request &, Response &));

        //with container
        void get(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        void post(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        void put(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        void patch(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        void Delete(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);
        void options(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container);

        std::optional<Route> findRoute(std::string request, JETPP::Methods method, std::string clientAddress);
    private:
        std::vector<Route> routes;
        std::vector<Container> container;
        void splitRoute(std::string str, std::vector<std::string> &segments, char delimiter);
        bool checkAccess(Container container, std::string clientAddress);
        void addRoute(const std::string &routeurl, JETPP::Methods method, void (*callback)(Request &, Response &), Container &container);
        void checkContainer(Container *container);

    };
}

#endif
