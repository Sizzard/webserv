#pragma once

#include "webserv.hpp"

class Client
{

private:
public:
    Client();
    Client(int fd);
    Client(Client const &to_cpy);
    Client &operator=(Client const &cpy);
    ~Client();

    int _fd;
    std::map<std::string, std::string> _header;
    std::string _body;
};