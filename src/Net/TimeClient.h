#pragma once
#include <Core/Types.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

class TimeClient
{
    void run(const String & ip, int port)
    {
        int socket_fd;
        sockaddr_in server_address{};

        if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cout << "socket error, errno: " <<  errno << std::endl;
            exit(errno);
        }

        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(reinterpret_cast<const char *>(&ip));
        server_address.sin_port = htons(port);

        if (connect(socket_fd, reinterpret_cast<sockaddr *>(&server_address), sizeof(server_address)) != 0)
        {
            std::cout << "Failed to connect server, errno: " <<  errno << std::endl;
            exit(errno);
        }

        std::cout << "Enter message: ";
        String msg;
        std::getline(std::cin, msg);

        char buffer[1024] = {0};

        if (recv(socket_fd, &buffer, 1024, 0) != 1)
        {
            std::cout << "Failed to receive, errno: " <<  errno << std::endl;
            exit(errno);
        }

        std::cout << "server response: " << buffer << std::endl;

        close(socket_fd);
    }
};


