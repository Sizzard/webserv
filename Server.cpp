#include "Includes/Server.hpp"

Server::Server()
{
   
   return;
}

Server::Server(Server const &cpy)
{
   *this = cpy;
   return;
}

Server &Server::operator=(Server const &cpy)
{
   //Copy what you want : this->name = cpy.name
   return *this;
}

Server::~Server()
{
   return;
}