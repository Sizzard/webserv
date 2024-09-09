#pragma once

#include "webserv.hpp"

class DefaultErrorPages
{

private:
    std::map<size_t, std::string> _pages;

public:
    DefaultErrorPages();
    DefaultErrorPages(DefaultErrorPages const &to_cpy);
    DefaultErrorPages &operator=(DefaultErrorPages const &cpy);
    ~DefaultErrorPages();

    std::string get_page(size_t const &pageNb);
};