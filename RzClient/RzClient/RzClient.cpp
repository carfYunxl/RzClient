#include "RzClient.h"
#include "Core/Log.hpp"

namespace RzLib
{
    RzClient::RzClient(const std::string& server_ip, uint32_t server_port)
        : m_serverIp(server_ip)
        , m_serverPort(server_port)
        , m_socket(INVALID_SOCKET)
    {

    }
    RzClient::RzClient(std::string&& server_ip, uint32_t&& server_port)
        : m_serverIp(std::move(server_ip))
        , m_serverPort(std::move(server_port))
        , m_socket(INVALID_SOCKET)
    {
    }

    RzClient::~RzClient()
    {
        closesocket(m_socket);
        WSACleanup();
    }

    bool RzClient::Init()
    {
        WSADATA data;
        if ( WSAStartup(MAKEWORD(2, 2), &data) < 0 )
        {
            Log(LogLevel::ERR, "winsock2 start error !");
            return false;
        }

        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (m_socket == INVALID_SOCKET)
        {
            Log(LogLevel::ERR, "create socket failed!");
            return false;
        }

        return true;

    }
    bool RzClient::Connect()
    {
        SOCKADDR_IN serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(m_serverPort);
        inet_pton(AF_INET, m_serverIp.c_str(), &serverAddr.sin_addr);

        if ( connect(m_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR )
        {
            Log(LogLevel::ERR, "Connect error, error code : ", WSAGetLastError());
            return false;
        }

        std::thread thread([=]() 
        {
            char recvBuf[1500]{ 0 };
            while (1)
            {
                int ret = recv(m_socket, recvBuf, 1500, 0);
                if ( ret == SOCKET_ERROR )
                {
                    Log(LogLevel::ERR, "Recv from server error, error code : ", WSAGetLastError());
                }
                else
                {
                    Log(LogLevel::INFO, "Recv from server : ", recvBuf);
                    memset(recvBuf, 0, ret);
                }
            }
        });

        thread.detach();

        //发送数据、接收数据
        char sendBuf[512]{0};
        while ( 1 )
        {
            std::cin.getline(sendBuf, 512);
            if (strlen(sendBuf) == 0)
            {
                continue;
            }

            if (!strcmp(sendBuf,"quit"))
            {
                break;
            }
            else if( send(m_socket, sendBuf, strlen(sendBuf), 0) == SOCKET_ERROR )
            {
                Log(LogLevel::ERR, "send buffer to server failed, error code : ", WSAGetLastError());
                continue;
            }

            memset(sendBuf,0,strlen(sendBuf));
        }

        return true;
    }
}
