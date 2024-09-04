#include "Includes/webserv.hpp"

int main(void)
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8080);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    connect(clientSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));

    std::string message = "Je suis le client !";

    send(clientSocket, message.c_str(), message.length(), 0);

    listen(clientSocket, 5);

    accept(clientSocket, NULL, NULL);

    char buffer[3000] = {0};

    recv(clientSocket, buffer, 3000, 0);

    std::cout << buffer << std::endl;

    close(clientSocket);

    return 0;
}