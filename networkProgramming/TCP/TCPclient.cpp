#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

using namespace std;

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"

int main()
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wSadata;
    if (WSAStartup(wVersionRequested, &wSadata) != 0)
    {
        cout << "WSAStartup failed: \n";
        return 0;
    }

    // STEP 1: Create a listening socket
    sockaddr_in tcpserverAddr;
    tcpserverAddr.sin_family = AF_INET;
    tcpserverAddr.sin_port = htons(SERVER_PORT);
    tcpserverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // Listen on any available interface

    SOCKET listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listensocket != INVALID_SOCKET)
    {
        cout << "socket function succeeded" << endl;
    }

    // STEP 2: Bind the listening socket
    if (bind(listensocket, (sockaddr *)&tcpserverAddr, sizeof(tcpserverAddr)) != SOCKET_ERROR)
        cout << "Bind API completed successfully" << endl;
    else
    {
        cout << "Bind failed with error: " << WSAGetLastError() << endl;
        return 0;
    }

    // STEP 3: Set up and connect to the server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout << "Connection failed with code " << WSAGetLastError() << endl;
        return 0;
    }
    else
        cout << "Connection completed successfully." << endl;

    // STEP 3: Send data (example)
    const char *message = "eXit12! 12a";
    int bytesSent = send(client, message, strlen(message), 0);
    if (bytesSent == SOCKET_ERROR)
    {
        cout << "Send failed with error: " << WSAGetLastError() << endl;
    }

    // STEP 4: Receive data (example)
    char buffer[1024];
    int bytesReceived = recv(client, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "Receive failed with error: " << WSAGetLastError() << endl;
    }
    else
    {
        buffer[bytesReceived] = '\0';
        cout << "Received: " << buffer << endl;
    }

    // STEP 5: Shutdown the connection (if needed)
    // shutdown(client, SD_BOTH); // SD_BOTH closes both send and receive

    // STEP 6: Close sockets and clean up
    closesocket(listensocket);
    closesocket(client);
    WSACleanup();

    return 0;
}