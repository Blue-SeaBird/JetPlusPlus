#include "request.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

namespace JETPP
{

    Request::Request(std::string requesturl, std::string routeurl, std::string request)
    {
        this->requesturl = requesturl;
        this->routeurl = routeurl;
        this->request = request;

        // split the urls
        splitString(this->routeurl, this->routeSplitted, '/');
        splitString(requesturl, this->requestSplitted, '/');

        this->setQuery();
        this->setParams();
        this->setBody();
    }

    void Request::setParams()
    {
        if (this->routeurl.find(':') != std::string::npos)
        {
            // check if the segments include a ':' (is dynamic)
            for (int i = 0; i < this->routeSplitted.size(); i++)
            {
                if (this->routeSplitted[i].find(":") != std::string::npos)
                {
                    this->params[this->routeSplitted[i].substr(1, this->routeSplitted[i].size() - 1)] = this->requestSplitted[i];
                    continue;
                }
            }
        }
    }

    void Request::setQuery()
    {
        if (this->requesturl.find('?') != std::string::npos)
        {
            // check if the segments include a '?' (has a query)
            for (int i = 0; i < this->requestSplitted.size(); i++)
            {
                if (this->requestSplitted[i].find("?") != std::string::npos)
                {
                    std::vector<std::string> splittedSegment;
                    for (int j = 0; j < requestSplitted[i].size(); j++)
                    {
                        if (requestSplitted[i].at(j) == '?')
                        {
                            splitString(this->requestSplitted[i].substr(j + 1, this->requestSplitted[i].size() - j + 1), splittedSegment, '&');
                        }
                    }

                    std::vector<std::string> splittedSegmentQuery;
                    for (int j = 0; j < splittedSegment.size(); j++)
                    {
                        splitString(splittedSegment[j], splittedSegmentQuery, '=');
                        this->query[splittedSegmentQuery[0]] = splittedSegmentQuery[1];
                        splittedSegmentQuery.clear();
                    }
                }
            }
        }
    }

    void Request::setBody()
    {
        size_t bodyStart = request.find("\r\n\r\n");
        if (bodyStart != std::string::npos)
        {
            size_t contentLengthPos = request.find("Content-Length: ");
            if (contentLengthPos != std::string::npos)
            {
                size_t bodyLength = std::stoi(request.substr(contentLengthPos + 16, bodyStart - contentLengthPos - 16));
                // Extract the request body
                this->body = request.substr(bodyStart + 4, bodyLength);
            }
        }
    }

    void Request::setHeaders()
    {
    }

    void Request::splitString(std::string str, std::vector<std::string> &segments, char delimiter)
    {
        std::istringstream isstream(str);
        std::string segment;
        while (std::getline(isstream, segment, delimiter))
        {
            segments.push_back(segment);
        }
    }
}