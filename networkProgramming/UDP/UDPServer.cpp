// cd UDP
// g++ UDPServer.cpp -o UDPServer.exe -lws2_32
// .\UDPServer.exe 8080

#include <iostream>
#include <winsock2.h> // Include the Windows Sockets API header

using namespace std;

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

        // Resolve domain name to IP address
        hostent *host = gethostbyname(buffer); // Get host information based on domain name
        if (host == nullptr)                   // Check if domain resolution was successful
        {
            cerr << "Unable to resolve the domain name" << endl; // Report an error if resolution fails
        }
        else
        {
            address = (in_addr *)host->h_addr;                              // Get the IPv4 address from the host information
            cout << "Resolved IP Address: " << inet_ntoa(*address) << endl; // Print the resolved IP address
        }

        // Send the resolved IP address back to the client
        sendto(serverSocket, inet_ntoa(*address), strlen(inet_ntoa(*address)), 0, (sockaddr *)&clientAddr, clientAddrLen); // Send data back to client
    }

    closesocket(serverSocket); // Close the socket
    WSACleanup();              // Clean up Winsock resources

    return 0; // Indicate successful program execution
}