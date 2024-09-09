#include "Includes/Client.hpp"

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
    // std::cout << "TEST 2 " << std::endl;
    return 400;
}

int is_valid_path(std::string const &line)
{
    if (line[0] != '/')
    {
        return 400;
    }
    return 200;
}

int is_valid_protocol(std::string const &line)
{
    if (line.compare("HTTP/1.0\r") == 0 || line.compare("HTTP/1.0\r\n") == 0 || line.compare("HTTP/1.0\n") == 0 || line.compare("HTTP/1.0") == 0 || line.compare("HTTP/1.1\r") == 0 || line.compare("HTTP/1.1\r\n") == 0 || line.compare("HTTP/1.1\n") == 0 || line.compare("HTTP/1.1") == 0)
    {
        return 200;
    }
    else if (line.compare("HTTP/") == 0)
    {
    }
    std::cout << line << std::endl;
    return 400;
}

int parse_first_line(Client &client, std::string const &line)
{
    std::vector<std::string> v = split(line);

    if (is_valid_method(v[0]) != 200)
    {
        client._isValidRequest = false;
        std::cout << "TEST 1" << std::endl;
        return FAILURE;
    }
    else if (is_valid_path(v[1]) != 200)
    {
        client._isValidRequest = false;
        std::cout << "TEST 2" << std::endl;

        return FAILURE;
    }
    else if (is_valid_protocol(v[2]) != 200)
    {
        client._isValidRequest = false;
        std::cout << "TEST 3" << std::endl;
        return FAILURE;
    }
    client._method = v[0];
    client._path = v[1];
    client._protocol = v[2];
    client._isValidRequest = true;

    return SUCCESS;
}

int parsing_header(Client &client, char *dataRecv)
{
    std::string data = dataRecv;
    std::string line;
    std::map<std::string, std::string> headers;

    std::stringstream ss;
    ss << data;
    std::getline(ss, line);

    if (parse_first_line(client, line) == FAILURE)
    {
        std::cout << "Ceci est une bad request" << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}

void add_new_client(std::vector<Client> &clients, int const &servSocket, int epoll_fd)
{
    int clientSocket = accept(servSocket, NULL, NULL);

    std::cout << green << "New client detected on socket " << clientSocket << reset << std::endl;
    clients.resize(clients.size() + 1);
    assign_client(clients, Client(clientSocket));

    set_socket_flags(clientSocket);

    epoll_event client_event;
    client_event.data.fd = clientSocket;
    client_event.events = EPOLLIN | EPOLLET | EPOLLOUT | EPOLLHUP | EPOLLRDHUP;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &client_event);
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
    event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, servSocket, &event) == -1)
    {
        perror("epoll_ctl");
        return 1;
    }

    std::vector<epoll_event> events(500);
    std::vector<Client> clients(5);
    char dataRecv[8001] = {0};
    DefaultErrorPages defaultErrorPages;

    while (g_sigint == false)
    {
        std::cout << "Waiting for clients" << std::endl;

        int n = epoll_wait(epoll_fd, events.data(), events.size(), -1);
        for (int i = 0; i < n; ++i)
        {
            std::memset(dataRecv, 0, sizeof(dataRecv));

            if (events[i].events & EPOLLRDHUP)
            {
                std::cout << red << "EPOLLRDHUP\n"
                          << "Closing connection on socket : " << clientID << reset << std::endl;
                close(clientID);
                clients[clientID].reinitialize_client();
                continue;
            }
            if (events[i].events & EPOLLIN)
            {
                std::cout << green << "EPOLLIN" << reset << std::endl;

                if (clientID == servSocket)
                {
                    add_new_client(clients, servSocket, epoll_fd);
                }
                else
                {
                    clients[clientID]._dataSize = recv(clientID, dataRecv, 8001, 0);
                    std::cout << "dataSize : " << clients[clientID]._dataSize << std::endl;
                    if (clients[clientID]._dataSize == -1)
                    {
                        ;
                    }
                    else if (clients[clientID]._dataSize == 0)
                    {
                        std::cout << red << "Client disconnected" << reset << std::endl;
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientID, NULL);
                        close(clientID);
                        clients[clientID].reinitialize_client();
                    }
                    else if (clients[clientID]._dataSize > 8000)
                    {
                        std::cout << red << "400 Bad Request on socket : " << clientID << reset << std::endl;

                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientID, NULL);
                        close(clientID);
                        clients[clientID].reinitialize_client();
                    }
                    else
                    {
                        std::cout << "Data received :" << yellow << std::endl
                                  << dataRecv << reset << std::endl;
                    }
                }
            }
            if (events[i].events & EPOLLOUT)
            {
                std::cout << cyan << "EPOLLOUT" << reset << std::endl;

                std::string header;
                std::string body;
                std::string wholeResponse;

                if (clients[clientID]._dataSize == -1 || dataRecv[0] == 0)
                {
                    continue;
                }
                else if (parsing_header(clients[clientID], dataRecv) == FAILURE)
                {
                    body = defaultErrorPages.get_page(400);
                    header = header_generator("400 Bad Request\n", body.length());
                    wholeResponse = header + "\r\n\r\n" + body;

                    std::cout
                        << "Sending to Socket " << clientID << " :\n"
                        << red << wholeResponse << reset << std::endl;
                    write(clientID, wholeResponse.c_str(), wholeResponse.size());
                    std::cout << red << "Closing connection on socket : " << clientID << reset << std::endl;
                    close(clientID);
                    clients[clientID].reinitialize_client();
                }

                if (clients[clientID]._isValidRequest == true)
                {
                    std::cout << green << "Request is valid" << reset << std::endl;

                    std::ifstream index("html/index.html");

                    std::getline(index, body, '\0');

                    index.close();

                    header = header_generator("200 OK\n", body.length());

                    wholeResponse = header + "\r\n\r\n" + body;

                    std::cout << "Sending to Socket " << clientID << " :\n"
                              << cyan << wholeResponse << reset << std::endl;

                    write(clientID, wholeResponse.c_str(), wholeResponse.size());
                    clients[clientID].reinitialize_client();
                }
            }
        }
    }
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        close(it->_fd);
    }
    clients.clear();
    close(servSocket);
    close(epoll_fd);
}