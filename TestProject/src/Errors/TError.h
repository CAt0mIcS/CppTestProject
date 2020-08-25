#pragma once

#include <exception>


class TError : public std::exception
{
public:
	TError(const char* file, unsigned int line, HRESULT code) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;
private:
	std::string TranslateErrorCode(HRESULT hr) const;

private:
	unsigned int m_Line;
	HRESULT m_HR;

	mutable std::string m_WhatBuffer;
};


#define T_ERROR(code) TError(__FILE__, __LINE__, code)
#define T_ERROR_LAST() TError(__FILE__, __LINE__, GetLastError())

