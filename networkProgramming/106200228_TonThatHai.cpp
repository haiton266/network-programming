#include <winsock2.h>
#include <WS2tcpip.h>
#include <bits/stdc++.h>

// need link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
    WORD wVersion = MAKEWORD(2, 2); // Specify the version of Winsock you want to use
                                    // 2.2 -> x=2, y=2
    WSADATA wsaData;
    if (WSAStartup(wVersion, &wsaData) != 0) // Phiên bản Winsock cần tải, Con trỏ trỏ đến cấu trúc LPWSADATA
    {
        cout << "WSAStartup failed with error" << GetLastError() << endl;
        return 1;
    }
    cout << "WSAStartup completed." << endl;

    char ip[] = "127.0.0.";
    char stCodeString[10];
    unsigned int stCode = 106200228;
    unsigned int num = (stCode % 255) + 1;
    sprintf(stCodeString, "%d", num);
    strcat(ip, stCodeString);
    cout << "IP address: " << ip << endl;

    // inet_addr
    unsigned long ip_addr1 = inet_addr(ip);
    if (ip_addr1 == INADDR_NONE)
    {
        printf("Can't convert IP by inet_addr\n");
    }
    else
    {
        cout << "Convert inet_addr completed:" << ip_addr1 << endl;
    }

    // inet_pton
    unsigned long ip_addr2;
    if (inet_pton(AF_INET, ip, &ip_addr2) == 1)
    {
        cout << "Convert inet_pton completed:" << ip_addr2 << endl;
    }
    else
    {
        cout << "Can't convert IP by inet_pton\n";
    }

    // Call WSACleanup when application finishes
    if (WSACleanup() != 0)
        cout << "Clean failed with error." << GetLastError() << endl;

    return 0;
}