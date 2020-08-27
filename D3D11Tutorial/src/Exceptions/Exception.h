#pragma once



namespace Except
{
	class Exception : public std::exception
	{
	public:
		Exception(int line, const char* file);
		virtual const char* what() const override;
		virtual const char* GetType() const;
		int GetLine() const;
		const std::string& GetFile() const;
		std::string GetOriginString() const;

	private:
		int m_Line;
		std::string m_File;

	protected:
		mutable std::string m_WhatBuffer;

	};
}


#define THWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr);
#define THWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError());

