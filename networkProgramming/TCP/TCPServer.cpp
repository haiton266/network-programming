#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string.h>

using namespace std;

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_MAXSIZE 1024

#pragma comment(lib, "Ws2_32.lib")

char *lowercase(const char *str)
{
    char *result = (char *)malloc(strlen(str) + 1);
    for (int i = 0; str[i] != '\0'; i++)
    {
        result[i] = tolower(str[i]);
    }
    result[strlen(str)] = '\0';
    return result;
}
char *process(const char *input)
{
    char *result = (char *)malloc(strlen(input) + 1);
    strcpy(result, "");

    for (int i = 0; i < strlen(input); i++)
    {
        if ((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z'))
        {
            char temp[2] = {input[i], '\0'};
            strcat(result, temp);
        }
    }

    char *temp = lowercase(result);
    // Convert result to lowercase
    // for (int i = 0; i < strlen(result); i++)
    // {
    //     temp[i] = tolower(result[i]);
    // }
    if (strcmp(temp, "exit") == 0 || strcmp(temp, "quit") == 0)
        strcpy(result, "good bye");
    free(temp);
    return result;
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
            char *message = process(buff);
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
