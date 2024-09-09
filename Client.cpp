#include "Includes/Client.hpp"

Client::Client() : _fd(0), _dataSize(0), _isVacant(false), _isValidRequest(false)
{
    return;
}

Client::Client(int fd) : _fd(fd), _dataSize(0), _isVacant(false), _isValidRequest(false)
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
    this->_dataSize = cpy._dataSize;
    this->_isValidRequest = cpy._isValidRequest;
    this->_isVacant = cpy._isVacant;
    this->_method = cpy._method;
    this->_path = cpy._path;
    this->_protocol = cpy._protocol;
    this->_header = cpy._header;
    this->_body = cpy._body;
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
    this->_isVacant = true;
    this->_dataSize = 0;
}

void assign_client(std::vector<Client> &clients, Client const &client)
{
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (it->_isVacant == true)
        {
            *it = client;
            return;
        }
    }
    clients.push_back(client);
}