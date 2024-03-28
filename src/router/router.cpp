#include "../../include/jetplusplus/router/router.hpp"
#include <sstream>

namespace jetpp
{

    Router::Router() {
        auto defaultContainer=std::make_shared<Container>();
        this->containers.push_back(defaultContainer);
    }

    void Router::get(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        this->addRoute(routeurl, jetpp::Methods::Get, callback, this->containers.front());
    }

    void Router::post(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        this->addRoute(routeurl, jetpp::Methods::Post, callback, this->containers.front());
    }

    void Router::put(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        this->addRoute(routeurl, jetpp::Methods::Put, callback, this->containers.front());
    }

    void Router::patch(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        this->addRoute(routeurl, jetpp::Methods::Patch, callback, this->containers.front());
    }

    void Router::Delete(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        this->addRoute(routeurl, jetpp::Methods::Delete, callback, this->containers.front());
    }

    void Router::options(const std::string &routeurl, void (*callback)(Request &, Response &))
    {
        this->addRoute(routeurl, jetpp::Methods::Options, callback, this->containers.front());
    }

    void Router::get(const std::string &routeurl, void (*callback)(Request &, Response &), std::shared_ptr<Container>& container)
    {
        this->addRoute(routeurl, jetpp::Methods::Get, callback, container);
    }

    void Router::post(const std::string &routeurl, void (*callback)(Request &, Response &), std::shared_ptr<Container>& container)
    {
        this->addRoute(routeurl, jetpp::Methods::Post, callback, container);
    }

    void Router::put(const std::string &routeurl, void (*callback)(Request &, Response &), std::shared_ptr<Container>& container)
    {
        this->addRoute(routeurl, jetpp::Methods::Put, callback, container);
    }

     void Router::patch(const std::string &routeurl, void (*callback)(Request &, Response &), std::shared_ptr<Container>& container)
    {
        this->addRoute(routeurl, jetpp::Methods::Patch, callback, container);
    }

     void Router::Delete(const std::string &routeurl, void (*callback)(Request &, Response &), std::shared_ptr<Container>& container)
    {
        this->addRoute(routeurl, jetpp::Methods::Delete, callback, container);
    }

    void Router::options(const std::string &routeurl, void (*callback)(Request &, Response &), std::shared_ptr<Container>& container)
    {
        this->addRoute(routeurl, jetpp::Methods::Options, callback, container);
    }


    void Router::checkContainer(std::shared_ptr<Container>& container)
    {
        for (const auto& existingContainer : this->containers)
        {
            if (container->getContainerId() == existingContainer->getContainerId())
            {
                return;
            }
        }

        this->containers.push_back(container);
    }


    
    void Router::addRoute(const std::string &routeurl, jetpp::Methods method, void (*callback)(Request &, Response &), std::shared_ptr<Container>& container){
        Route route(routeurl, method, callback);

        this->checkContainer(container);
        container->addRoute(route);
    }

    std::optional<Route> Router::findRoute(std::string request, jetpp::Methods method, std::string clientAddress)
    {
        for (auto& container : this->containers)
        {
            std::vector<Route> routes = container->getRoutes();
            for (auto& route : routes)
            {
                if (route.getMethod() == method)
                {
                    if (request == route.getName()) // if it's not a dynamic URL, check if the request URL is absolutely equal to a route
                    {
                        bool access = this->checkAccess(container, clientAddress);
                        if (access)
                        {
                            return route;
                        }
                        return std::nullopt;
                    }
                    else if (route.getName().find(':') != std::string::npos)
                    {
                        bool match = true;

                        // split the URLs
                        std::vector<std::string> routeSplitted;
                        std::vector<std::string> requestSplitted;
                        splitRoute(route.getName(), routeSplitted, '/');
                        splitRoute(request, requestSplitted, '/');

                        // check for an equal amount of segments
                        if (routeSplitted.size() != requestSplitted.size())
                        {
                            continue;
                        }

                        // check if the segments are equal except for :[name]
                        for (size_t j = 0; j < routeSplitted.size(); j++)
                        {
                            if (routeSplitted[j].find(":") != std::string::npos)
                            {
                                continue;
                            }
                            else if (routeSplitted[j] != requestSplitted[j]) // if segments are unequal, break and match=false
                            {
                                match = false;
                                break;
                            }
                        }

                        // if match is true, return the route
                        if (match)
                        {
                            bool access = this->checkAccess(container, clientAddress);
                            if (access)
                            {
                                return route;
                            }
                            return std::nullopt;
                        }
                    }
                    else if (request.find("?") != std::string::npos)
                    {
                        std::vector<std::string> routeSplitted;
                        splitRoute(request, routeSplitted, '/');

                        std::string url = "";
                        for (size_t j = 0; j < routeSplitted.size(); j++)
                        {
                            if (routeSplitted[j].find("?") == std::string::npos)
                            {
                                url += routeSplitted[j];
                            }
                            else
                            {
                                for (size_t k = 0; k < routeSplitted[j].size(); k++)
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
                        if (route.getName() == url)
                        {
                            bool access = this->checkAccess(container, clientAddress);
                            if (access)
                            {
                                return route;
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

    bool Router::checkAccess(std::shared_ptr<Container>& container, std::string clientAddress) {
        if (container->getAccessList().empty()) return true;

        for (const auto& address : container->getAccessList())
        {
            if (address == clientAddress)
            {
                return true; // Client address found in the access list
            }
        }
        return false;
    }
}
