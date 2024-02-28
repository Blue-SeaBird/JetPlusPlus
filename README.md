# JetPlusPlus

C++ REST API Framework

Created and developed at Blue SeaBird by: @Peeentaa

# Documentation for JetPlusPlus

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Getting Started](#getting-started)
5. [Routes and Endpoints](#routes-and-endpoints)
   - [GET](#get-route)
   - [POST](#post-route)
6. [Containers](#containers)

## Introduction

JetPlusPlus is a C++ REST API framework designed to simplify the development of robust and scalable web services. This documentation provides an overview of the features and usage of JetPlusPlus, along with examples to guide you through the process.

## Features

- **Modular Routing**: Easily define routes with callback functions, making it simple to handle various endpoints in your application.
- **Container Support**: JetPlusPlus now supports containers to restrict access to specified hosts, enhancing security for your API.
- **JSON Response Handling**: The framework provides built-in support for handling JSON responses, making it convenient to work with JSON data in your API.


## Installation

To use JetPlusPlus in your C++ project, follow these steps:

1. Clone the JetPlusPlus repository.
2. Include the necessary headers in your project.
3. Build and link your project with JetPlusPlus.

## Getting Started

Create a main function in your C++ application and define your API endpoints using the JetPlusPlus router.

```cpp
#include "router/router.hpp"
#include "server/server.hpp"

int main() {
    Router router;
    router.get("/your-endpoint",[](JETPP::Request &req, JETPP::Response &res)
     { 
        //your logic here
     });
    
    Server server(router);
    server.start(8080);
    return 0;
}
```

## Containers


### Creating a Container

To create a container, use the JETPP::Container class:

```cpp
JETPP::Container myContainer;
````

### Adding Access Hosts

You can specify hosts that are allowed to access your API. Use the addAccessHost method:

```cpp
myContainer.addAccessHost("127.0.0.1");
```

This example allows access only from the host with the IP address "127.0.0.1". You can add multiple hosts as needed.

### Associating Containers with Routes

Associate a container with a specific route to apply access restrictions. Pass the container as the last argument when defining a route:

```cpp
router.get("/restricted-endpoint", [](JETPP::Request &req, JETPP::Response &res)
{
    // Your logic here
}, myContainer);
```

Now, the "/restricted-endpoint" route will only be accessible from hosts specified in the associated container.


### Code example with container

```cpp
int main()
{
    JETPP::Router router;

    // ... Your existing route definitions ...

    // Define a container and add an access host
    JETPP::Container userProxyContainer;
    userProxyContainer.addAccessHost("127.0.0.1:3000");

    // Associate the container with a route
    router.get("/users/:user", [](JETPP::Request &req, JETPP::Response &res)
               { 
                    std::string message="Success for users and :user= "+req.params["user"];
                    res.send(message); }, userProxyContainer);

    // ... Your existing route definitions ...

    JETPP::Server server(router);
    server.start(8000);

    return 0;
}
```
