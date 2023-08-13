#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <thread>
#include <string>

namespace RzLib
{
    constexpr size_t MAX_TCP_PACKAGE_SIZE = 1500;

    class RzClient
    {
    public:
        RzClient(const std::string& server_ip, uint32_t server_port);
        RzClient(std::string&& server_ip, uint32_t&& server_port);

        ~RzClient();

        const SOCKET GetSocket() const { return m_socket; }
        const std::string GetVersion() const { return m_Version; }

        bool Recv();
        bool Send();

        bool Init();
        bool Connect();

        bool RecvFile(size_t fileSize, const std::string& filepath);

        bool UpdateClient();

    private:
        std::string m_serverIp;
        uint32_t    m_serverPort;
        SOCKET      m_socket;

        std::string m_Version;
    };
}