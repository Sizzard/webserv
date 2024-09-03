#include "Includes/webserv.hpp"

void check_args(int ac, char **env)
{
    if (ac != 2)
    {
        std::cerr << "Usage : ./webserv [configuration file]" << std::endl;
        exit(1);
    }
    (void)env;
}

void parsing_config_file(char **av)
{
    (void)av;
    // Parser le fichier de config av[1] et exit ou throw si il y a une erreur;
}

void signal_handler(int signal)
{
    (void)signal;
    exit(1);
}

int main(int ac, char **av, char **env)
{
    std::signal(SIGINT, signal_handler);
    check_args(ac, env);
    parsing_config_file(av);

    int servSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in servAddr;
    socklen_t servAddrLen = sizeof(servAddr);

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8080);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(servSocket, (struct sockaddr *)&servAddr, servAddrLen);

    listen(servSocket, 5);

    std::vector<pollfd> fds;
    pollfd servFds{servSocket, POLLIN, 0};
    fds.push_back(servFds);

    while (true)
    {
        poll(fds.data(), fds.size(), -1);

        for (size_t i = 0; i < fds.size(); ++i)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == servSocket)
                {
                    int clientSocket = accept(servSocket, nullptr, nullptr);
                    pollfd client_pollfd = {clientSocket, POLLIN, 0};
                    fds.push_back(client_pollfd);
                    std::cout << "Nouveau client connecte !" << std::endl;
                }
            }
        }
    }
    close(servSocket);

    return 0;
}