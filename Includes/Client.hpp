#pragma once

#include "webserv.hpp"
#include "DefaultErrorPages.hpp"

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
    int _dataSize;
    bool _isVacant;
    bool _isValidRequest;
    std::string _method;
    std::string _path;
    std::string _protocol;
    std::map<std::string, std::string> _header;
    std::string _body;
};

void assign_client(std::vector<Client> &clients, Client const &client);