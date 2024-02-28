#include "../../include/jetplusplus/container/container.hpp"

namespace JETPP{
    Container::Container(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10);

        std::stringstream ss;
        ss << "Container_" << dis(gen);
        containerId = ss.str();
    }

    void Container::addRoute(Route route){
        this->routes.push_back(route);
    }

    void Container::addAccessHost(std::string host){
        this->accessList.push_back(host);
    }

    std::vector<Route> Container::getRoutes(){
        return this->routes;
    }

    std::vector<std::string> Container::getAccessList(){
        return this->accessList;
    }

    std::string Container::getContainerId(){
        return this->containerId;
    }
}