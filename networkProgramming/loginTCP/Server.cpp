#include <winsock2.h>
#include <ws2tcpip.h>
#include <bits/stdc++.h>

using namespace std;

#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_MAXSIZE 1024

#pragma comment(lib, "Ws2_32.lib")

struct UserData
{
    string id;
    string password;
    bool active;
    bool login;
};
bool isLogin = false;
vector<UserData> userData;
string saveUserLogged;

vector<UserData> readUserCSV(string filename)
{
    vector<UserData> userDataRead;
    ifstream file(filename);

    if (!file.is_open())
        cerr << "Error: Unable to open file " << filename << endl;

    string line;

    // Skip the header line
    getline(file, line);

    while (getline(file, line))
    {
        stringstream lineStream(line);
        string cell;
        UserData user;

        // Read id, password, active, and login
        if (getline(lineStream, user.id, ',') &&
            getline(lineStream, user.password, ',') &&
            getline(lineStream, cell, ',') &&
            stringstream(cell) >> user.active &&
            getline(lineStream, cell, ',') &&
            stringstream(cell) >> user.login)
        {
            userDataRead.push_back(user);
        }
        else
        {
            cerr << "Error: Invalid data format in line" << endl;
        }
    }
    file.close();
    return userDataRead;
}

void updateUserCSV(string filename)
{
    ofstream file(filename, ios::out | ios::trunc);
    if (!file.is_open())
    {
        cerr << "Error: Unable to create/open file " << filename << endl;
        return;
    }
    file << "ID,Password,Active,Login\n";
    for (const UserData &user : userData)
    {
        file << user.id << "," << user.password << "," << user.active << "," << user.login << "\n";
    }
    file.close();
}

string process(const string &input)
{
    userData = readUserCSV("database.csv"); // Update data from database
    istringstream iss(input);
    string request, id, password, temp;
    iss >> request >> id >> password >> temp;

    if (request == "login" && id != "" && password != "" && temp == "")
    {
        if (isLogin == 1)
            return "You logged in";
        for (UserData &user : userData)
        {
            if (user.id == id)
            {
                if (user.password == password)
                {
                    isLogin = 1;
                    saveUserLogged = user.id;
                    user.login = 1;
                    updateUserCSV("database.csv");
                    return "Login successfully";
                }
                return "Wrong password";
            }
        }
        return "Id does not exist";
    }
    else if (request == "logout" && id == "") // Check if redundant params
    {
        if (isLogin == 0)
            return "Please login";
        for (auto &user : userData)
            if (user.id == saveUserLogged)
                user.login = 0;
        isLogin = false;
        updateUserCSV("database.csv");
        return "Logout successfully";
    }
    else
        return "Wrong message format";
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
    if (listenSock == INVALID_SOCKET)
    {
        cout << "Creating socket failed with code " << WSAGetLastError() << endl;
        WSACleanup(); // Clean up before returning
        return 1;
    }
    cout << "Creating socket completed successfully.\n";

    sockaddr_in tcpServerAddr;
    tcpServerAddr.sin_family = AF_INET;
    tcpServerAddr.sin_port = htons(SERVER_PORT);
    tcpServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (bind(listenSock, (sockaddr *)&tcpServerAddr, sizeof(tcpServerAddr)) == SOCKET_ERROR)
    {
        cout << "Bind API failed with code " << WSAGetLastError() << endl;
        closesocket(listenSock); // Clean up before returning
        WSACleanup();
        return 1;
    }
    cout << "Bind API completed successfully.\n";

    if (listen(listenSock, 5) == SOCKET_ERROR)
    {
        cout << "Listen failed with code " << WSAGetLastError() << endl;
        closesocket(listenSock); // Clean up before returning
        WSACleanup();
        return 1;
    }
    cout << "Server is listening for requests..." << endl;

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
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
    clientPort = ntohs(clientAddr.sin_port);
    cout << "Connection is established: IP = " << clientIP << " at port = " << clientPort << endl;

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

    shutdown(NewConnection, SD_BOTH);

    closesocket(NewConnection);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
