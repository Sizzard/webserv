#pragma once

#include "webserv.hpp"

class Server
{

private:
    sockaddr_in _servAddr;
    std::vector<epoll_event> _event;

public:
    Server();
    Server(Server const &to_cpy);
    Server &operator=(Server const &cpy);
    ~Server();
};