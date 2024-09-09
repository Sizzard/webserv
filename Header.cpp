#include "Includes/webserv.hpp"
#include "Includes/DefaultErrorPages.hpp"

std::string get_time()
{
    std::string result;

    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "Date: %a, %d %b %G %T %Z\n", timeinfo);
    result = buffer;

    return result;
}

std::string header_generator(std::string const &code, size_t bodyLength)
{
    std::string header("HTTP/1.1 ");

    header += code;
    header += "Server: WebServ 1.0\n";
    header += get_time();
    header += "Content-Type: text/html\n";
    header += "Content-Length: ";

    std::stringstream ss;

    ss << bodyLength;

    header += ss.str();

    return header;
}