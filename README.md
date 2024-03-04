# JetPlusPlus
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![CMake](https://img.shields.io/badge/CMake-064F8C.svg?style=for-the-badge&logo=CMake&logoColor=white) ![Awesome](https://awesome.re/badge.svg)

C++ REST API Framework

Created and developed at Blue SeaBird by: @Peeentaa

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Getting Started](#getting-started)
5. [Routes and Endpoints](#routes-and-endpoints)
   - [GET](#get)
   - [POST](#post)
6. [Containers](#containers)

## Introduction

JetPlusPlus is a C++ REST API framework designed to simplify the development of robust and scalable web services. This documentation provides an overview of the features and usage of JetPlusPlus, along with examples to guide you through the process.

## Features

- **Modular Routing**: Easily define routes with callback functions, making it simple to handle various endpoints in your application.
- **Container Support**: JetPlusPlus supports proxy containers to restrict access to specified hosts, enhancing security for your API.
- **JSON Response Handling**: The framework provides built-in support for handling JSON responses, making it convenient to work with JSON data in your API.


## Installation

To use JetPlusPlus in your C++ project, follow these steps:

1. Clone the JetPlusPlus repository.
```sh
git clone https://github.com/Blue-SeaBird/JetPlusPlus.git
```
2. Include the necessary headers in your project.
3. Build and link your project with JetPlusPlus.

## Getting Started

Create a main function in your C++ application and define your API endpoints using the JetPlusPlus router.

```cpp
#include "jetplusplus/router/router.hpp"
#include "jetplusplus/server/server.hpp"

int main() {
    JETPP::Router router;
    router.get("/your-endpoint",[](JETPP::Request &req, JETPP::Response &res)
     { 
        //your logic here
     });
    
    JETPP::Server server(router);
    server.start(8080);
    return 0;
}
```

## Routes and Endpoints

### GET

Simple GET route:
```cpp
router.get("/users", [](JETPP::Request &req, JETPP::Response &res)
    {
        res.send("Hello World"); 
    });

```

With custom status:
```cpp
router.get("/users", [](JETPP::Request &req, JETPP::Response &res)
    {
        res.status(200).send("Hello World"); 
    });
```


With json response:
```cpp
router.get("/users", [](JETPP::Request &req, JETPP::Response &res)
    {
        JETPP::JsonValue users;
        JETPP::JsonValue user;
        user.setObject({{"name", JETPP::JsonValue("John Doe")}});

        std::vector<JETPP::JsonValue> usersVector;
        users.setArray(userVector)
        res.json(users);
    });
```

With dynamic route:
```cpp
router.get("/users/:userid", [](JETPP::Request &req, JETPP::Response &res)
    {
        std::string userid=req.params.userid;
        res.status(200).send(userid);
    });
```

With query params:

```cpp
router.get("/users", [](JETPP::Request &req, JETPP::Response &res)
    {
        std::string userid=req.query.userid;
        res.status(200).send(userid);
    });
```

```http
### Expect: 123 (STATUSCODE 200)
GET http://localhost:8080/users?userid=123
```

### POST

Simple POST route:
```cpp
router.post("/players", [](JETPP::Request &req, JETPP::Response &res)
    {
        res.send(200);
    });
```

Retrieve the body:
```cpp
router.post("/players", [](JETPP::Request &req, JETPP::Response &res)
    {
        JETPP::JsonValue body=req.body;
        res.send(200);
    });
```

### PUT

Simple PUT route:
```cpp
router.put("/players", [](JETPP::Request &req, JETPP::Response &res)
    {
        res.send(200);
    });
```

### PATCH

Simple PATCH route:
```cpp
router.patch("/players", [](JETPP::Request &req, JETPP::Response &res)
    {
        res.send(200);
    });
```

### DELETE

Simple DELETE route:
```cpp
router.Delete("/players", [](JETPP::Request &req, JETPP::Response &res)
    {
        res.send(200);
    });
```

### OPTIONS

Simple OPTIONS route:
```cpp
router.post("/players", [](JETPP::Request &req, JETPP::Response &res)
    {
        res.send(200);
    });
```

## Containers


### Creating a Container

To create a container, use the JETPP::Container class:

```cpp
JETPP::Container myContainer;
```

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
#include "jetplusplus/router/router.hpp"
#include "jetplusplus/server/server.hpp"
#include "jetplusplus/container/container.hpp"

int main()
{
    JETPP::Router router;

    // ... Your existing route definitions ...

    // Define a container and add an access host
    JETPP::Container userProxyContainer;
    userProxyContainer.addAccessHost("127.0.0.1");

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
