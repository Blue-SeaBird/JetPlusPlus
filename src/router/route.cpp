#include "route.hpp"
#include <string>
#include <stdio.h>

namespace JETPP
{
    Route::Route(std::string name, JETPP::Methods method, std::function<void(Request &, Response &)> callback)
    {
        this->name = name;
        this->method = method;
        this->callback = callback;
    }

    std::string Route::getName()
    {
        return this->name;
    }

    JETPP::Methods Route::getMethod()
    {
        return this->method;
    }

    void Route::execute(Request &request, Response &response)
    {
        this->callback(request, response); // Execute the callback function
    }
}
