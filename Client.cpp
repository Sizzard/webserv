#include "Includes/Client.hpp"

Client::Client() : _fd(0), _isValidRequest(false)
{
    return;
}

Client::Client(int fd) : _fd(fd), _isValidRequest(false)
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

void Client::reinitialize_client()
{
    this->_method.clear();
    this->_path.clear();
    this->_protocol.clear();
    this->_header.clear();
    this->_body.clear();
    this->_isValidRequest = false;
}