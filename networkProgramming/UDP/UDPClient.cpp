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

    const char *serverIP = argv[1];   // Get server IP from command line argument
    int port = atoi(argv[2]);         // Get port number from command line argument
    const char *domainName = argv[3]; // Get domain name from command line argument

    // Initialize Winsock
    WSADATA wsaData;                               // Structure to hold details about Windows socket implementation
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Initialize Winsock library
    {
        cerr << "WSAStartup failed!" << endl; // Report an error if initialization fails
        return 1;
    }

    // Create socket
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0); // Create a UDP socket
    if (clientSocket == INVALID_SOCKET)                   // Check if socket creation was successful
    {
        cerr << "Socket creation failed!" << endl; // Report an error if socket creation fails
        WSACleanup();                              // Clean up Winsock resources
        return 1;
    }

    // Set up server address
    sockaddr_in serverAddr;                           // Structure to hold server address information
    serverAddr.sin_family = AF_INET;                  // Set address family to IPv4
    serverAddr.sin_port = htons(port);                // Set the port number
    serverAddr.sin_addr.s_addr = inet_addr(serverIP); // Set the server IP address

    // Send domain name to server
    if (sendto(clientSocket, domainName, strlen(domainName), 0, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) // Send data to server
    {
        cerr << "Error in sendto()" << endl; // Report an error if sending fails
        closesocket(clientSocket);           // Close the socket
        WSACleanup();                        // Clean up Winsock resources
        return 1;
    }

    char buffer[1024];                              // Buffer to hold received data
    sockaddr_in serverResponse;                     // Structure to hold server response information
    int serverResponseLen = sizeof(serverResponse); // Length of the server response structure

    // Receive the resolved IP address from the server
    int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr *)&serverResponse, &serverResponseLen); // Receive data from server
    if (bytesReceived == SOCKET_ERROR)                                                                                      // Check for errors in receiving data
    {
        cerr << "Error in recvfrom()" << endl; // Report an error if receiving fails
    }
    else
    {
        buffer[bytesReceived] = '\0';                      // Null-terminate the received data
        cout << "Resolved IP Address: " << buffer << endl; // Print the resolved IP address
    }

    closesocket(clientSocket); // Close the socket
    WSACleanup();              // Clean up Winsock resources

    return 0; // Indicate successful program execution
}
