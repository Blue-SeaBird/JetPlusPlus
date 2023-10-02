#include "response.hpp"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>

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

    // https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header
    Response::Response(_WINSOCK2_H::SOCKET clientSocket)
    {
        this->clientSocket = clientSocket;
    }

    void Response::send(std::string message)
    {
        // Build and send the message
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(message.length()) + "\r\n\r\n" + message;
        _WINSOCK2_H::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0);
    }

    void Response::sendFile(std::string path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file" << std::endl;
            return;
        }

        std::string fileextension = path.substr(path.rfind('.') + 1); // get the fileextension

        auto it = mimeTypesToFileExtensions.find(fileextension); // get the content-type

        file.seekg(0, std::ios::end);
        size_t filesize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(filesize);
        file.read(buffer.data(), filesize);

        // craft the response message
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: ";
        if (it != mimeTypesToFileExtensions.end())
        {
            response += it->second;
        }
        else
        {
            std::cerr << "Unsupported file version" << std::endl;
        }
        response += "\r\nContent-Length: " + std::to_string(filesize) + "\r\n\r\n";
        response += std::string(buffer.begin(), buffer.end());

        _WINSOCK2_H::send(this->clientSocket, response.c_str(), strlen(response.c_str()), 0); // send the response
        file.close();
    }

    void Response::status(int status)
    {
        this->statuscode = status;
    }
}