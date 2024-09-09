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

    void reinitialize_client();

    int _fd;
    bool _isValidRequest;
    std::string _method;
    std::string _path;
    std::string _protocol;
    std::map<std::string, std::string> _header;
    std::string _body;
};