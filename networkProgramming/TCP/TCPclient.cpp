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
    cout << "WSAStartup completed." << endl;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == INVALID_SOCKET)
    {
        cout << "Creating socket failed with code " << WSAGetLastError() << endl;
        WSACleanup(); // Clean up before returning
        return 1;
    }
    cout << "Creating socket completed successfully.\n";

    if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cout << "Connection failed with code " << WSAGetLastError() << endl;
        return 0;
    }
    cout << "Connection completed successfully." << endl;

    // STEP 3: Send data (example)
    const char *message = "eXit2a@";
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
    shutdown(client, SD_BOTH); // SD_BOTH closes both send and receive

    // STEP 6: Close sockets and clean up
    closesocket(client);
    WSACleanup();

    return 0;
}