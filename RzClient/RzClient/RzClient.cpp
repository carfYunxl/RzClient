#include "RzClient.h"
#include "Core/Log.hpp"
#include "Core/RzThread.h"
#include <fstream>

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

        Log(LogLevel::INFO, "Connect to server success!");

        Recv();
        Send();

        return true;
    }

    bool RzClient::Recv()
    {
        ScopeThread sThread(std::thread([=]()
        {
            std::string strRecv;
            while (1)
            {
                strRecv.resize(1500);
                int ret = recv(m_socket, &strRecv[0], 1500, 0);
                strRecv.resize(ret);
                if (ret == SOCKET_ERROR)
                {
                    Log(LogLevel::ERR, "Recv from server error, error code : ", WSAGetLastError());
                    return false;
                }
                else
                {
                    if (strRecv.starts_with("file"))
                    {
                        //说明服务器要给我发送文件了
                        size_t index1 = strRecv.find(" ",0);
                        size_t index2 = strRecv.find(" ",index1 + static_cast<size_t>(1));

                        std::string strFileSize = strRecv.substr(index1 + static_cast<size_t>(1), index2 - index1 - static_cast<size_t>(1));
                        int fileSize = 0;
                        if (!strFileSize.empty())
                        {
                            fileSize = stoi(strFileSize);
                        }

                        // get file name
                        std::string strFilePath = strRecv.substr(index2+ static_cast<size_t>(1), strRecv.size() - index2 - static_cast<size_t>(1));
                        size_t idx = strFilePath.rfind("\\");
                        if(idx != std::string::npos)
                            strFilePath = strFilePath.substr(idx+2, strFilePath.size() - idx - 2);

                        std::string strFileCache;
                        while ( fileSize > 0 )
                        {
                            memset(&strRecv[0],0,1024);
                            ret = recv(m_socket, &strRecv[0], 1024, 0);

                            if (ret == SOCKET_ERROR)
                            {
                                Log(LogLevel::ERR, "Recv file from server failed!, error code : ", WSAGetLastError());
                                return false;
                            }

                            strFileCache += strRecv;

                            fileSize -= 1024;
                        }

                        std::ofstream ofs(strFilePath, std::ios::out);
                        if (ofs.is_open())
                        {
                            ofs.write(strFileCache.c_str(), strFileCache.size());
                            ofs.flush();
                            ofs.close();

                            Log(LogLevel::INFO, "Recv file from server success!");
                        }
                    }
                    else
                    {
                        Log(LogLevel::INFO, "server say : ", strRecv);
                    }
                    memset(&strRecv[0], 0, ret);
                }
            }
        }));

        return true;
    }

    bool RzClient::Send()
    {
        char readBuf[1500]{0};

        while (1)
        {
            std::cin.getline(readBuf,1500);

            if (strlen(readBuf) == 0)
            {
                continue;
            }

            if (strcmp(readBuf,"exit") == 0)
            {
                break;
            }
            else if (send(m_socket, readBuf, strlen(readBuf), 0) == SOCKET_ERROR)
            {
                Log(LogLevel::ERR, "send buffer to server failed, error code : ", WSAGetLastError());
                continue;
            }

            memset(readBuf, 0, 1500);
        }

        return true;
    }
}
