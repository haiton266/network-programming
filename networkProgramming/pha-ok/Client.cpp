// g++ UDPclient.cpp -o UDPclient.exe -lws2_32
// .\UDPclient.exe 127.0.0.1 8080 dut.udn.vn

#include <iostream>
#include <winsock2.h>

using namespace std;

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
int main()
{
    const char *serverIP = SERVER_ADDR;
    int port = SERVER_PORT;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "WSAStartup failed!" << endl;
        return 1;
    }
    cout << "WSAStartup completed." << endl;

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        cerr << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }
    cout << "Creating socket completed successfully.\n";

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    string messageSend;
    while (1)
    {
        cout << "(Client) -->";
        getline(cin, messageSend);
        const char *message = messageSend.c_str();
        if (sendto(clientSocket, message, strlen(message), 0, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            cerr << "Error in sendto()" << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        char buffer[1024];
        sockaddr_in serverResponse;
        int serverResponseLen = sizeof(serverResponse);

        int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr *)&serverResponse, &serverResponseLen);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recvfrom()" << endl;
        }
        else
        {
            buffer[bytesReceived] = '\0';
            cout << "(Server): " << buffer << endl;
        }
    }
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
