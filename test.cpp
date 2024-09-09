#include "Includes/DefaultErrorPages.hpp"

int main(void)
{
    // for (size_t i = 0; i < 10; i++)
    // {
    //     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    //     sockaddr_in servAddr;
    //     servAddr.sin_family = AF_INET;
    //     servAddr.sin_port = htons(8080);
    //     servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //     connect(clientSocket, (struct sockaddr *)&servAddr, sizeof(servAddr));

    //     std::string message = "Je suis le client !";

    //     // send(clientSocket, message.c_str(), message.length(), 0);

    //     // sleep(1);

    //     // listen(clientSocket, 5);

    //     // accept(clientSocket, NULL, NULL);
    //     sleep(1);
    // }
    // sleep(3);
    return 0;
}


// Content-type: text/html
// Content-length: 444
// \r\n
// 444huhfughugu

// Transfer-Enconding: chunked
// \r\n 
// 45\r\n 
// 45bf45gkgghkg\r\n 
// 12\r\n 
// 12gjgichghg\r\n 
// 0\r\n 
// \r\n 

// Content-type: multipart/form-data; --boundarystring=delimiter
// \n 
// --delimter\n 
// Content-type: text/html\n
// \n
// sifhgdjhgjfjs\r\n 
// --delimter--\n