#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <regex>

int main(void)
{
    char buffer[50] = "";
    std::string line = buffer;
    std::string word;
    std::vector<std::string> v;
    std::regex test;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; end != std::string::npos; i++)
    {
        begin = line.find_first_not_of(" ", i);
        end = line.find_first_of(" ", begin);
        if (begin > line.length())
            break;
        word = line.substr(begin, end - begin);
        v.push_back(word);
        i = end;
    }
    return 0;
}