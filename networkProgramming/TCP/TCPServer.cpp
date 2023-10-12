#include <winsock2.h>
#include <ws2tcpip.h>
#include <bits/stdc++.h>

using namespace std;

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_MAXSIZE 1024

#pragma comment(lib, "Ws2_32.lib")

string process(string input)
{
    string output = "";
    for (char c : input)
        if (isalpha(c))
            output += tolower(c);
    return (output == "exit" || output == "quit") ? "good bye" : output;
}

int main()
{
    WORD wVersion = MAKEWORD(2, 2);
    WSADATA wsaData;

    if (WSAStartup(wVersion, &wsaData) != 0)
    {
        cout << "WSAStartup failed with error " << GetLastError() << endl;
        return 1;
    }
    cout << "WSAStartup completed." << endl;

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock != INVALID_SOCKET)
        cout << "Creating socket completed successfully.\n";
    else
    {
        cout << "Creating socket failed with code " << WSAGetLastError() << endl;
        WSACleanup(); // Clean up before returning
        return 1;
    }

    sockaddr_in tcpServerAddr;
    tcpServerAddr.sin_family = AF_INET;
    tcpServerAddr.sin_port = htons(SERVER_PORT);
    tcpServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (bind(listenSock, (sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr)) != SOCKET_ERROR)
        cout << "Bind API completed successfully.\n";
    else
    {
        cout << "Bind API failed with code " << WSAGetLastError() << endl;
        closesocket(listenSock); // Clean up before returning
        WSACleanup();
        return 1;
    }

    if (listen(listenSock, 5) != SOCKET_ERROR)
        cout << "Server is listening for requests..." << endl;
    else
    {
        cout << "Listen failed with code " << WSAGetLastError() << endl;
        closesocket(listenSock); // Clean up before returning
        WSACleanup();
        return 1;
    }

    sockaddr_in clientAddr;
    char buff[BUFF_MAXSIZE], clientIP[INET_ADDRSTRLEN];
    int ret, clientAddrLen = sizeof(clientAddr), clientPort;

    SOCKET NewConnection = accept(listenSock, (sockaddr *)&clientAddr, &clientAddrLen);
    if (NewConnection == INVALID_SOCKET)
    {
        cout << "Connection failed with code: " << WSAGetLastError() << endl;
        closesocket(listenSock); // Clean up before returning
        WSACleanup();
        return 1;
    }
    else
    {
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        clientPort = ntohs(clientAddr.sin_port);
        cout << "Connection is established: IP = " << clientIP << " at port = " << clientPort << endl;
    }

    while (1)
    {
        ret = recv(NewConnection, buff, BUFF_MAXSIZE, 0);
        if (ret == SOCKET_ERROR)
        {
            cout << "Error with code " << WSAGetLastError() << endl;
            break;
        }
        else if (ret == 0)
        {
            cout << "Client disconnected." << endl;
            break;
        }
        else
        {
            buff[ret] = '\0'; // Null-terminate the received data
            cout << "Received message from client " << clientIP << ":" << clientPort << ": " << buff << endl;
            string buffStr(buff);
            buffStr = process(buffStr);
            const char *message = buffStr.c_str();
            ret = send(NewConnection, message, strlen(message), 0);
            if (ret == SOCKET_ERROR)
            {
                cout << "Error with code " << WSAGetLastError() << endl;
                break;
            }
        }
    }

    closesocket(NewConnection);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
