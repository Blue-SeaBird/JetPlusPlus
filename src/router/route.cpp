#include "../../include/jetplusplus/router/route.hpp"
#include <string>
#include <stdio.h>

namespace jetpp
{
    Route::Route(std::string routeurl, jetpp::Methods method, std::function<void(Request &, Response &)> callback)
    {
        this->routeurl = routeurl;
        this->method = method;
        this->callback = callback;
    }

    std::string Route::getRouteurl()
    {
        return this->routeurl;
    }

    jetpp::Methods Route::getMethod()
    {
        return this->method;
    }

    void Route::execute(Request &request, Response &response)
    {
        try
        {
            this->callback(request, response); // Execute the callback function
        }
        catch (const std::runtime_error &)
        {
            std::cout << "Couldn't execute callback function of route: " << this->getRouteurl() << std::endl;
        }
    }
}
