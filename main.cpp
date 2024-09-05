#include "Includes/webserv.hpp"

bool g_sigint = false;

void signal_handler(int signal)
{
    (void)signal;
    std::cout << red << "\nSIGINT detected" << reset << std::endl;
    g_sigint = true;
}

void check_args(int ac, char **ev)
{
    if (ac != 2)
    {
        std::cerr << "Usage : ./webserv [configuration file]" << std::endl;
        exit(FAILURE);
    }
    (void)ev;
}

void parsing_config_file(char **av)
{
    (void)av;
    // Parser le fichier de config av[1] et exit ou throw si il y a une erreur;
}

int set_socket_flags(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
    {
        perror("fctnl");
        return FAILURE;
    }

    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
    {
        perror("fctnl");
        return FAILURE;
    }
    return SUCCESS;
}

std::vector<std::string> split(std::string const &line)
{
    std::string word;
    std::vector<std::string> v;

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
    return v;
}

int is_valid_method(std::string const &line)
{
    if (line.compare("GET") == 0)
    {
        // std::cout << "TEST 2 " << std::endl;
        return 200;
    }
    else if (line.compare("POST") == 0)
    {
        return 200;
    }
    else if (line.compare("DELETE") == 0)
    {
        return 200;
    }
    return 400;
}

int is_valid_path(std::string const &line)
{
    if (line[0] != '/')
    {
        return 200;
    }
    return 400;
}

int is_valid_protocol(std::string const &line)
{
    if (line.compare("HTTP/1.0") == 0 || line.compare("HTTP/1.1") == 0)
    {
        return 200;
    }
    else if (line.compare("HTTP/") == 0 )
    {
        
    }
    return 400;
}

int parse_first_line(std::string const &line)
{
    std::vector<std::string> v = split(line);

    if (is_valid_method(v[0]) != 200)
    {
        return FAILURE;
    }
    else if (is_valid_path(v[1]) != 200)
    {
        return FAILURE;
    }
    else if (is_valid_protocol(v[2]) != 200)
    {
        return FAILURE;
    }

    return SUCCESS;
}

int parsing_header(char *dataRecv)
{
    std::string data = dataRecv;
    std::string line;
    std::map<std::string, std::string> headers;

    std::stringstream ss;
    ss << data;
    std::getline(ss, line);

    if (parse_first_line(line) == false)
    {
        std::cout << "Ceci est une bad request" << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}

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

std::string header_generator(int bodyLength)
{
    std::string header("HTTP/1.1 ");

    header += "200 OK\n";
    header += "Server: WebServ 1.0\n";
    header += get_time();
    header += "Content-Type: text/html\n";
    header += "Content-Length: ";

    std::stringstream ss;

    ss << bodyLength;

    header += ss.str();

    return header;
}

int main(int ac, char **av, char **ev)
{
    check_args(ac, ev);
    parsing_config_file(av);
    std::signal(SIGINT, signal_handler);

    int servSocket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    setsockopt(servSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8080);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(servSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));

    listen(servSocket, 500);

    if (set_socket_flags(servSocket))
    {
        return FAILURE;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        return FAILURE;
    }

    struct epoll_event event;

    event.data.fd = servSocket;
    event.events = EPOLLIN | EPOLLET;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, servSocket, &event) == -1)
    {
        perror("epoll_ctl");
        return 1;
    }

    std::vector<epoll_event> events(500);
    std::vector<int> clientSockets(500);

    while (g_sigint == false)
    {
        int n = epoll_wait(epoll_fd, events.data(), events.size(), -1);
        for (int i = 0; i < n; ++i)
        {
            if (events[i].events & POLLIN)
            {
                if (events[i].data.fd == servSocket)
                {
                    int clientSocket = accept(servSocket, NULL, NULL);

                    std::cout << green << "New client detected" << reset << std::endl;

                    clientSockets.push_back(clientSocket);

                    set_socket_flags(clientSocket);

                    epoll_event client_event;
                    client_event.data.fd = clientSocket;
                    client_event.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &client_event);
                    char dataRecv[3000] = {0};
                    recv(clientSocket, dataRecv, 3000, 0);
                    std::cout << "Data received :" << yellow << std::endl
                              << dataRecv << reset << std::endl;
                    if (dataRecv[0] == 0)
                    {
                        continue;
                    }
                    else if (parsing_header(dataRecv) == FAILURE)
                    {
                        // 400 Bad Request connection closed;
                        // exit(1);
                    }
                    else
                    {

                        std::ifstream index("site/index.html");

                        std::string body;

                        std::getline(index, body, '\0');

                        index.close();

                        std::string header = header_generator(body.length());

                        std::string wholeResponse = header + "\r\n\r\n" + body;

                        std::cout << "Sending :\n"
                                  << wholeResponse << std::endl;

                        write(clientSocket, wholeResponse.c_str(), wholeResponse.size());
                    }
                }
            }
        }
    }
    for (size_t i = 0; i != clientSockets.size(); ++i)
    {
        close(clientSockets[i]);
    }
    close(servSocket);
    close(epoll_fd);
}
