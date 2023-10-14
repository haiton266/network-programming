// cd UDP
// g++ UDPServer.cpp -o UDPServer.exe -lws2_32
// .\UDPServer.exe 8080

#include <bits/stdc++.h>
#include <winsock2.h> // Include the Windows Sockets API header
#include <stdio.h>
#include <ws2tcpip.h>

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
        else
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(rp->ai_family, addr, ipstr, sizeof(ipstr));
        string s(ipstr);
        st += "\n" + s;
    }

    freeaddrinfo(result);
    return st;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: serverUDP.exe <port_number>" << endl;
        return 1;
    }

    int port = atoi(argv[1]); // Get the port number from command line argument

    // Initialize Winsock
    WSADATA wsaData;                               // Structure to hold details about Windows socket implementation
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Initialize Winsock library
    {
        cerr << "WSAStartup failed!" << endl; // Report an error if initialization fails
        return 1;
    }

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket
    if (serverSocket == INVALID_SOCKET)                   // Check if socket creation was successful
    {
        cerr << "Socket creation failed!" << endl; // Report an error if socket creation fails
        return 1;
    }

    // Bind socket to the specified port
    sockaddr_in serverAddr;                  // Structure to hold server address information
    serverAddr.sin_family = AF_INET;         // Set address family to IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    serverAddr.sin_port = htons(port);       // Set the port number

    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) // Bind socket to the specified port
    {
        cerr << "Binding failed!" << endl; // Report an error if binding fails
        closesocket(serverSocket);         // Close the socket
        WSACleanup();                      // Clean up Winsock resources
        return 1;
    }

    char buffer[1024];                      // Buffer to hold received data
    sockaddr_in clientAddr;                 // Structure to hold client address information
    int clientAddrLen = sizeof(clientAddr); // Length of the client address structure
    in_addr *address;                       // Pointer to a structure representing an IPv4 address

    while (true)
    {
        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr *)&clientAddr, &clientAddrLen); // Receive data from client
        if (bytesReceived == SOCKET_ERROR)                                                                              // Check for errors in receiving data
        {
            cerr << "Error in recvfrom()" << endl; // Report an error if receiving fails
            break;                                 // Exit the loop
        }

        buffer[bytesReceived] = '\0'; // Null-terminate the received data

        string st = process(buffer);
        const char *message = st.c_str();

        sendto(serverSocket, message, strlen(message), 0, (sockaddr *)&clientAddr, clientAddrLen); // Send data back to client
    }

    closesocket(serverSocket); // Close the socket
    WSACleanup();              // Clean up Winsock resources

    return 0; // Indicate successful program execution
}