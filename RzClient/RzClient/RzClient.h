#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <thread>
#include <string>

namespace RzLib
{
    class RzClient
    {
    public:
        RzClient(const std::string& server_ip, uint32_t server_port);
        RzClient(std::string&& server_ip, uint32_t&& server_port);

        bool Init();
        bool Connect();

        ~RzClient();

    private:
        std::string m_serverIp;
        uint32_t m_serverPort;
        SOCKET m_socket;
    };
}
