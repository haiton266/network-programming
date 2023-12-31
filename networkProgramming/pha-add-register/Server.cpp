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
map<int, string> loginWith;
vector<UserData> userData;

vector<UserData> readUserCSV(string filename)
{
    vector<UserData> userDataRead;
    ifstream file(filename);

    if (!file.is_open())
        cerr << "Error: Unable to open file " << filename << endl;

    string line;

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

string process(const string &input, int clientPort)
{
    userData = readUserCSV("database.csv"); // Update data from database
    istringstream iss(input);
    string request, id, password, temp;
    iss >> request >> id >> password >> temp;
    if (request == "register" && id != "" && password != "" && temp == "")
    {
        if (loginWith[clientPort] != "")
            return "You logged in";
        for (UserData &user : userData)
            if (user.id == id)
                return "Id existing";
        userData.push_back({id, password, 1, 0});
        updateUserCSV("database.csv");
        return "Register successfully";
    }
    else if (request == "login" && id != "" && password != "" && temp == "")
    {
        if (loginWith[clientPort] != "")
            return "You logged in";
        for (UserData &user : userData)
        {
            if (user.id == id)
            {
                if (user.password == password)
                {
                    // Bỏ cmt nếu muốn 1 tài khoản chỉ vào được 1 client duy nhất (Code hiện tại: 1 tài khoản vào được nhiều client như Fb)
                    // if (user.login == 0)
                    // {
                    loginWith[clientPort] = id;
                    user.login = 1;
                    updateUserCSV("database.csv");
                    return "Login successfully";
                    // }
                    // else
                    //     return "You logged at another client";
                }
                else
                    return "Wrong password";
            }
        }
        return "Id does not exist";
    }
    else if (request == "password" && id != "" && password != "" && temp == "")
    {
        if (loginWith[clientPort] == "")
            return "Please logged in";
        string newPassword = id;
        string reEnterPw = password;
        if (newPassword == reEnterPw)
        {
            for (UserData &user : userData)
                if (user.id == loginWith[clientPort])
                    user.password = newPassword;
            updateUserCSV("database.csv");
            return "Change password successfully";
        }
        return "The 2 passwords are not the same";
    }
    else if (request == "logout" && id == "") // Check if redundant params
    {
        if (loginWith[clientPort] == "")
            return "Please login";
        for (auto &user : userData)
            if (user.id == loginWith[clientPort])
                user.login = 0;
        loginWith[clientPort] = "";
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

    SOCKET udpServerSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (udpServerSock == INVALID_SOCKET)
    {
        cout << "Creating socket failed with code " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    cout << "Creating socket completed successfully.\n";

    sockaddr_in udpServerAddr;
    udpServerAddr.sin_family = AF_INET;
    udpServerAddr.sin_port = htons(SERVER_PORT);
    udpServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (bind(udpServerSock, (sockaddr *)&udpServerAddr, sizeof(udpServerAddr)) == SOCKET_ERROR)
    {
        cout << "Bind API failed with code " << WSAGetLastError() << endl;
        closesocket(udpServerSock);
        WSACleanup();
        return 1;
    }
    cout << "Bind API completed successfully.\n";

    sockaddr_in clientAddr;
    char buff[BUFF_MAXSIZE], clientIP[INET_ADDRSTRLEN];
    int ret, clientAddrLen = sizeof(clientAddr), clientPort;

    while (1)
    {
        ret = recvfrom(udpServerSock, buff, BUFF_MAXSIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
        if (ret == SOCKET_ERROR)
        {
            cout << "Error with code " << WSAGetLastError() << endl;
            break;
        }
        else
        {
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
            clientPort = ntohs(clientAddr.sin_port);
            buff[ret] = '\0';
            cout << "Received message from client " << clientIP << ":" << clientPort << ": " << buff << endl;
            string buffStr(buff);
            buffStr = process(buffStr, clientPort);
            const char *message = buffStr.c_str();
            ret = sendto(udpServerSock, message, strlen(message), 0, (sockaddr *)&clientAddr, clientAddrLen);
            if (ret == SOCKET_ERROR)
            {
                cout << "Error with code " << WSAGetLastError() << endl;
                break;
            }
        }
    }

    closesocket(udpServerSock);
    WSACleanup();
    return 0;
}
