#include "../../include/jetplusplus/router/router.hpp"
#include <sstream>

namespace JETPP
{

    Router::Router() {
        Container defaultContainer;
        this->container.push_back(defaultContainer);
    }

    void Router::get(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        get(routeurl, callback ,this->container.front());
    }

    void Router::post(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        post(routeurl, callback ,this->container.front());
    }

    void Router::options(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        options(routeurl, callback ,this->container.front());
    }


    void Router::get(const std::string &routeurl, void (*callback)(Request &, Response &),Container &container)
    {
        Route route(routeurl, JETPP::Methods::Get, callback);
        Container &checkedContainer = checkContainer(container);

        checkedContainer.addRoute(route);
    }

    void Router::post(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container)
    {
        Route route(routeurl, JETPP::Methods::Post, callback);
        Container &checkedContainer = checkContainer(container);

        checkedContainer.addRoute(route);
    }

    void Router::options(const std::string &routeurl, void (*callback)(Request &, Response &), Container &container)
    {
        Route route(routeurl, JETPP::Methods::Options, callback);
        Container &checkedContainer = checkContainer(container);

        checkedContainer.addRoute(route);
    }

    Container &Router::checkContainer(Container &container)
    {
        for (int i = 0; i < this->container.size(); i++)
        {
            if (container.getContainerId() == this->container[i].getContainerId())
            {
                return this->container[i];
            }
        }
        this->container.push_back(container);
        return this->container.back();
    }


    std::optional<Route> Router::findRoute(std::string request, JETPP::Methods method, std::string clientAddress)
    {
        for(int cI=0;cI<this->container.size();cI++){
            std::vector<Route> routes = this->container[cI].getRoutes();
            for (int i = 0; i < routes.size(); i++)
            {
                if (routes[i].getMethod() == method)
                {

                    if (request == routes[i].getName()) // if its not a dynamic url check if the request url is absolute equal to a route
                    {
                        bool access=this->checkAccess(this->container[cI], clientAddress);
                            if(access){
                                return routes[i];
                            }
                            return std::nullopt;
                    }
                    else if (routes[i].getName().find(':') != std::string::npos)
                    {
                        bool match = true;

                        // split the urls
                        std::vector<std::string> routeSplitted;
                        std::vector<std::string> requestSplitted;
                        splitRoute(routes[i].getName(), routeSplitted, '/');
                        splitRoute(request, requestSplitted, '/');

                        // check for an equal amount of segments
                        if (routeSplitted.size() != requestSplitted.size())
                        {
                            continue;
                        }

                        // check if the segments are equal except for :[name]
                        for (int j = 0; j < routeSplitted.size(); j++)
                        {
                            if (routeSplitted[j].find(":") != std::string::npos)
                            {
                                continue;
                            }
                            else if (routeSplitted[j] != requestSplitted[j]) // if segments are unequal break and match=false
                            {
                                match = false;
                                break;
                            }
                        }

                        // if match is true reutrn the route
                        if (match)
                        {
                            bool access=this->checkAccess(this->container[cI], clientAddress);
                            if(access){
                                return routes[i];
                            }
                            return std::nullopt;
                        }
                    }
                    else if (request.find("?")!=std::string::npos)
                    {
                        std::vector<std::string> routeSplitted;
                        splitRoute(request, routeSplitted, '/');

                        std::string url = "";
                        for (int j = 0; j < routeSplitted.size(); j++)
                        {
                            if (routeSplitted[j].find("?") == std::string::npos)
                            {
                                url += routeSplitted[j];
                            }
                            else
                            {
                                for (int k = 0; k < routeSplitted[j].size(); k++)
                                {
                                    if (routeSplitted[j].at(k) == '?')
                                    {
                                        url += "/";
                                        url += routeSplitted[j].substr(0, k);
                                        break;
                                    }
                                }
                            }
                        }
                        if (routes[i].getName() == url)
                        {
                            bool access=this->checkAccess(this->container[cI], clientAddress);
                            if(access){
                                return routes[i];
                            }
                            return std::nullopt;
                        }
                    }
                }
            }
        }
        throw std::runtime_error("Route not found");
    }

    void Router::splitRoute(std::string str, std::vector<std::string> &segments, char delimiter)
    {
        std::istringstream isstream(str);
        std::string segment;
        while (std::getline(isstream, segment, delimiter))
        {
            segments.push_back(segment);
        }
    }

    bool Router::checkAccess(Container container, std::string clientAddress) {
        if (container.getAccessList().empty()) return true;

        size_t colonPos = clientAddress.find(":");
        std::string addressWithoutPort = (colonPos != std::string::npos) ? clientAddress.substr(0, colonPos) : clientAddress;
        std::cout << "Without port" << addressWithoutPort << std::endl;

        for (int i = 0; i < container.getAccessList().size(); i++) {
            if (container.getAccessList()[i] == addressWithoutPort) {
                return true;
            }
        }
        return false;
    }
}
