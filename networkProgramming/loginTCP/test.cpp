#include <iostream>
#include <sstream>
#include <string>

std::string process(const std::string &input)
{
    std::istringstream iss(input);

    std::string request;
    iss >> request;
    std::string id;
    iss >> id;
    std::string password;
    iss >> password;
    return request;
}

int main()
{
    std::string inputString = "GET 123 mypassword"; // Example input

    std::string result = process(inputString);

    std::cout << "Request: " << result << std::endl;

    return 0;
}
