#include "router.hpp"
#include <sstream>

namespace JETPP
{

    Router::Router() {}

    void Router::get(const std::string &routeurl, std::function<void(Request &, Response &)> callback)
    {
        Route route(routeurl, JETPP::Methods::Get, callback);
        this->routes.push_back(route);
    }

    void Router::post(const std::string &routeurl, std::function<void(Request &, Response &)> callback)
    {
        Route route(routeurl, JETPP::Methods::Post, callback);
        this->routes.push_back(route);
    }

    std::vector<Route> Router::getRoutes()
    {
        return this->routes;
    }

    Route Router::findRoute(std::string request, JETPP::Methods method)
    {
        std::vector<Route> routes = this->routes;
        for (int i = 0; i < routes.size(); i++)
        {
            if (routes[i].getMethod() == method)
            {

                if (request == routes[i].getName()) // if its not a dynamic url check if the request url is absolute equal to a route
                {
                    return routes[i];
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
                        return routes[i];
                    }
                }
                else if (request.find("?"))
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
                        return routes[i];
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
}
