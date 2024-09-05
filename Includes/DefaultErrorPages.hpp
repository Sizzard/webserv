#pragma once

#include <webserv.hpp>

class DefaultErrorPages
{

private:
    std::vector<std::string> _pages;

public:
    DefaultErrorPages();
    DefaultErrorPages(DefaultErrorPages const &to_cpy);
    DefaultErrorPages &operator=(DefaultErrorPages const &cpy);
    ~DefaultErrorPages();
};