// g++ UDPclient.cpp -o UDPclient.exe -lws2_32
// .\UDPclient.exe 127.0.0.1 8080 quora.com

#include <iostream>
#include <winsock2.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Usage: clientUDP.exe <server_ip> <port_number> <domain_name>" << endl;
        return 1;
    }

    const char *serverIP = argv[1];
    int port = atoi(argv[2]);
    const char *domainName = argv[3];

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "WSAStartup failed!" << endl;
        return 1;
    }
    cout << "WSAStartup successfully!" << endl;

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        cerr << "Socket creation failed!" << endl;
        WSACleanup();
        return 1;
    }
    cout << "Socket creates successfully!" << endl;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    if (sendto(clientSocket, domainName, strlen(domainName), 0, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
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
        cout << "Resolved IP Address: " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
