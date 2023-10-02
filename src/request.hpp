#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace JETPP
{
    class Request
    {
    private:
        std::vector<std::string> routeSplitted;
        std::vector<std::string> requestSplitted;
        std::string request;
        std::string routeurl;
        void setQuery();
        void setParams();
        void splitString(std::string str, std::vector<std::string> &segments, char delimiter);

    public:
        std::unordered_map<std::string, std::string> params;
        std::unordered_map<std::string, std::string> query;
        Request(std::string request, std::string routeurl);
    };
}

#endif
