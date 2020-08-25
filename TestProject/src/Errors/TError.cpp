#include "pch.h"
#include "TError.h"

TError::TError(const char* file, unsigned int line, HRESULT code) noexcept
    :m_Line(line), m_HR(code), m_WhatBuffer("")
{

}

const char* TError::what() const noexcept
{
    std::stringstream ss;
    ss << GetType() << '\n'
        << "[Error Code] " << GetErrorCode() << '\n'
        << "[Description] " << GetErrorString() << '\n';

    m_WhatBuffer = ss.str();
    return m_WhatBuffer.c_str();
}

const char* TError::GetType() const noexcept
{
    return "Test Window Exception";
}

HRESULT TError::GetErrorCode() const noexcept
{
    return m_HR;
}

std::string TError::GetErrorString() const noexcept
{
    return TranslateErrorCode(m_HR);
}

std::string TError::TranslateErrorCode(HRESULT hr) const
{
    char* buff;
    DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buff, 0, nullptr);

    if (!nMsgLen)
    {
        return "Unidentified error code";
    }

    std::string errorStr = buff;
    LocalFree(buff);
    return errorStr;
}
