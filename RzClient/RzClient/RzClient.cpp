#include "RzClient.hpp"
#include "Core/Log.hpp"
#include "Core/RzThread.hpp"
#include <fstream>
#include "CMD/CMDParser.hpp"
#include <memory>
#include "CMD/CMDType.hpp"

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
            strRecv.resize(MAX_TCP_PACKAGE_SIZE);
            while (1)
            {
                int ret = recv(m_socket, &strRecv[0], MAX_TCP_PACKAGE_SIZE, 0);
                if (ret == SOCKET_ERROR)
                {
                    int ret = WSAGetLastError();
                    if (ret == WSAECONNABORTED)
                    {
                        print(" Disconnect from server! ");
                    }
                    else
                    {
                        Log(LogLevel::ERR, "Recv from server error, error code : ",ret );
                    }
                    return false;
                }
                else
                {
                    strRecv.resize(strlen(&strRecv[0]));

                    CMDParser parser(strRecv);

                    std::unique_ptr<CMD> pCMD;

                    std::string strCmd = parser.GetCMD();

                    switch (parser.GetCmdType())
                    {
                        case CMDType::NONE:
                            continue;
                        case CMDType::SINGLE:
                        {
                            pCMD = std::make_unique<CMDSingle>(strCmd, this);
                            break;
                        }
                        case CMDType::DOUBLE:
                        {
                            pCMD = std::make_unique<CMDDouble>(strCmd, this, parser.GetSecInfo());
                            break;
                        }
                        case CMDType::TRIPLE:
                        {
                            pCMD = std::make_unique<CMDTriple>(strCmd, this, parser.GetSecInfo(), parser.GetMsg());
                            break;
                        }
                    }

                    pCMD->Run();
                    memset(&strRecv[0], 0, MAX_TCP_PACKAGE_SIZE);
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
            else if ( send(m_socket, readBuf, int(strlen(readBuf)), 0) == SOCKET_ERROR )
            {
                Log(LogLevel::ERR, "send buffer to server failed, error code : ", WSAGetLastError());
                continue;
            }

            memset(readBuf, 0, 1500);
        }

        return true;
    }

    bool RzClient::RecvFile(size_t fileSize, const std::string& filepath)
    {
        std::string strFileCache;
        std::string strRecv;
        strRecv.resize(MAX_TCP_PACKAGE_SIZE);
        while (fileSize > 0)
        {
            memset(&strRecv[0], 0, MAX_TCP_PACKAGE_SIZE);
            int ret = recv(m_socket, &strRecv[0], MAX_TCP_PACKAGE_SIZE, 0);

            if (ret == SOCKET_ERROR)
            {
                Log(LogLevel::ERR, "Recv file from server failed!, error code : ", WSAGetLastError());
                return false;
            }

            size_t len = strlen(&strRecv[0]);
            if (len != MAX_TCP_PACKAGE_SIZE)
                strFileCache += strRecv.substr(0, len);
            else
                strFileCache += strRecv;

            Log(LogLevel::INFO, "recv file success, total = ", strFileCache.size());

            fileSize -= MAX_TCP_PACKAGE_SIZE;
        }

        std::ofstream ofs(filepath, std::ios::out);
        if (ofs.is_open())
        {
            ofs.write(strFileCache.c_str(), strFileCache.size());
            ofs.flush();
            ofs.close();

            Log(LogLevel::INFO, "Recv file from server success!");
        }

        return true;
    }

    bool RzClient::UpdateClient()
    {
        std::string strUpdate("update");
        if (SOCKET_ERROR == send(m_socket, &strUpdate[0],strUpdate.size(),0))
        {
            return false;
        }
        return true;
    }

    //接收服务器发过来的可执行档
    bool RzClient::RecvExe(size_t fileSize, const std::string& filepath)
    {
        return true;
    }
}
