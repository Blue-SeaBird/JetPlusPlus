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
        std::string requesturl;
        std::string routeurl;
        void setQuery();
        void setParams();
        void setHeaders();
        void splitString(std::string str, std::vector<std::string> &segments, char delimiter);

    public:
        std::unordered_map<std::string, std::string> params;
        std::unordered_map<std::string, std::string> query;
        std::unordered_map<std::string, std::string> headers;
        Request(std::string requesturl, std::string routeurl);
    };
}

#endif
