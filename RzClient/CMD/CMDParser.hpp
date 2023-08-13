/*****************************************************************//**
 * \file   CMDParser.hpp
 * \brief  Several classes to handle cmd input from console.
 *              - handle server command
 *              - handle client command
 * 
 * \author yun
 * \date   August 2023
 *********************************************************************/
#pragma once
#include <string>
#include <winsock2.h>
#include "Core/Core.hpp"

namespace RzLib
{
    enum class CMDType
    {
        NONE = 0,
        SINGLE,
        DOUBLE,
        TRIPLE
    };

    class CMDParserBase
    {
    public:
        CMDParserBase(const std::string& CMD, char SPLIT = ' ');
        virtual ~CMDParserBase() {}

        virtual void SetCMD(const std::string& CMD, char SPLIT = ' ');
        std::string  GetCMD() const { return m_CMD; }

    protected:
        virtual void Parser(const std::string& CMD, char SPLIT = ' ');
    protected:
        std::string m_CMD;
    };

    class CMDParser : public CMDParserBase
    {
    public:
        CMDParser(const std::string& CMD, char SPLIT = ' ');
        ~CMDParser() {}
        std::string GetSecInfo()    const { return m_SecInfo; }
        std::string GetCMD()        const { return m_CMD; }
        std::string GetMsg()        const { return m_message; }
        CMDType     GetCmdType()    const { return m_cmdType; }

        virtual void SetCMD(const std::string& CMD, char SPLIT = ' ') override;

    private:
        virtual void Parser(const std::string& CMD, char SPLIT = ' ') override;
        bool IsAllDigits(const std::string& digits);
    private:
        std::string m_SecInfo;
        std::string m_message;
        CMDType     m_cmdType{ CMDType::NONE };
    };
}
