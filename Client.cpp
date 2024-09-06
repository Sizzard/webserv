#include "Includes/Client.hpp"

Client::Client()
{
    return;
}

Client::Client(int fd) : _fd(fd)
{
    return;
}

Client::Client(Client const &cpy)
{
    *this = cpy;
    return;
}

Client &Client::operator=(Client const &cpy)
{
    this->_fd = cpy._fd;
    // Copy what you want : this->name = cpy.name
    return *this;
}

Client::~Client()
{
    return;
}