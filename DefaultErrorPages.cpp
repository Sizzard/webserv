#include "Includes/DefaultErrorPages.hpp"

DefaultErrorPages::DefaultErrorPages()
{
    this->_pages[400] =
        "<html>\n"
        "<head><title>400 Bad Request</title></head>\n"
        "<body>\n"
        "<center><h1>400 Bad Request</h1></center>\n"
        "<hr><center>WebServ/1.0</center>\n"
        "</body>\n"
        "</html>";
    return;
}

DefaultErrorPages::DefaultErrorPages(DefaultErrorPages const &cpy)
{
    *this = cpy;
    return;
}

DefaultErrorPages &DefaultErrorPages::operator=(DefaultErrorPages const &cpy)
{
    this->_pages = cpy._pages;
    return *this;
}

DefaultErrorPages::~DefaultErrorPages()
{
    return;
}

std::string DefaultErrorPages::get_page(size_t const &pageNb)
{
    return this->_pages[pageNb];
}