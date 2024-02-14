#include "response.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <unistd.h>

namespace JETPP
{
    std::unordered_map<std::string, std::string> mimeTypesToFileExtensions = {
        {".jar", "application/java-archive"},
        {".x12", "application/EDI-X12"},
        {".edi", "application/EDIFACT"},
        {".js", "application/javascript"},
        {".bin", "application/octet-stream"},
        {".ogg", "application/ogg"},
        {".pdf", "application/pdf"},
        {".xhtml", "application/xhtml+xml"},
        {".swf", "application/x-shockwave-flash"},
        {".json", "application/json"},
        {".jsonld", "application/ld+json"},
        {".xml", "application/xml"},
        {".zip", "application/zip"},
        {".url", "application/x-www-form-urlencoded"},
        {".mp3", "audio/mpeg"},
        {".wma", "audio/x-ms-wma"},
        {".ra", "audio/vnd.rn-realaudio"},
        {".wav", "audio/x-wav"},
        {".gif", "image/gif"},
        {".jpeg", "image/jpeg"},
        {".jpg", "image/jpeg"},
        {".png", "image/png"},
        {".tiff", "image/tiff"},
        {".txt", "text/plain"}};

    // Constructor
    Response::Response(int clientSocket)
    {
        this->clientSocket = clientSocket;
    }

    // Add a header to the response
    void Response::addHeader(const std::string &key, const std::string &value)
    {
        this->header.push_back(key + ": " + value);
    }

    // Send a string as the response
    void Response::send(std::string message)
    {
        try
        {
            // Build the response message with header
            std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(message.length()) + "\r\nAccess-Control-Allow-Origin: *";

            for (const std::string &header : header)
            {
                response += "\r\n" + header;
            }

            response += "\r\n\r\n" + message;

            // Send the response
            ::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0);
        }
        catch (const std::runtime_error &)
        {
            // Handle errors
            std::string response = "HTTP/1.1 500 Internal server error\r\nContent-Length: 0\r\n\r\n";
            ::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0);
        }
    }

    // Send a file as the response
    void Response::sendFile(std::string path)
    {
        try
        {
            std::ifstream file(path, std::ios::binary);
            if (!file.is_open())
            {
                std::cerr << "Failed to open file" << std::endl;
                return;
            }

            std::string fileextension = path.substr(path.rfind('.') + 1);

            auto it = mimeTypesToFileExtensions.find(fileextension);
            file.seekg(0, std::ios::end);
            size_t filesize = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<char> buffer(filesize);
            file.read(buffer.data(), filesize);

            // Build the response message with header
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: ";
            if (it != mimeTypesToFileExtensions.end())
            {
                response += it->second;
            }
            else
            {
                std::cerr << "Unsupported file version" << std::endl;
            }
            response += "\r\nContent-Length: " + std::to_string(filesize);
            response += "\r\nAccess-Control-Allow-Origin: *";

            for (const std::string &header : header)
            {
                response += "\r\n" + header;
            }

            response += "\r\n\r\n";
            response += std::string(buffer.begin(), buffer.end());

            // Send the response
            ::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0);
            file.close();
        }
        catch (const std::runtime_error &)
        {
            // Handle errors
            std::string response = "HTTP/1.1 500 Internal server error\r\nContent-Length: 0\r\n\r\n";
            ::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0);
        }
    }

    // Send a JSON object as the response
    void Response::json(JETPP::JsonValue object)
    {
        try
        {
            std::string message = object.toJsonString();

            // Build the response message with header
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + std::to_string(message.length()) + "\r\nAccess-Control-Allow-Origin: *";

            for (const std::string &header : header)
            {
                response += "\r\n" + header;
            }

            response += "\r\n\r\n" + message;

            // Send the response
            ::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0);
        }
        catch (const std::runtime_error &)
        {
            // Handle errors
            std::string response = "HTTP/1.1 500 Internal server error\r\nContent-Length: 0\r\n\r\n";
            ::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0);
        }
    }

    // Set the status code for the response
    JETPP::Response Response::status(int status)
    {
        this->statuscode = status;
        return *this;
    }
}
