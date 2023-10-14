#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <bits/stdc++.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

string process(char *hostname)
{
    string st = "";
    struct addrinfo hints, *result, *rp;
    ZeroMemory(&hints, sizeof(hints));
    // hints.ai_family = AF_UNSPEC;     // Allow IPv4 or IPv6
    hints.ai_family = AF_INET;       // Allow IPv4
    hints.ai_socktype = SOCK_STREAM; // Use TCP

    int status = getaddrinfo(hostname, NULL, &hints, &result);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        WSACleanup();
        return "Not found information";
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];

        if (rp->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        // else
        // {
        //     struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
        //     addr = &(ipv6->sin6_addr);
        // }

        inet_ntop(rp->ai_family, addr, ipstr, sizeof(ipstr));
        string s(ipstr);
        st = st + s + "\n";
    }

    freeaddrinfo(result);

    return st;
}

int main()
{
    char *hostname = "quora.com";

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup failed\n");
        return 1;
    }

    cout << process(hostname);

    WSACleanup();

    return 0;
}
