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


	class WindowException : public Exception
	{
	public:
		WindowException(int line, const char* file, HRESULT hr);
		virtual const char* what() const override;
		virtual const char* GetType() const override;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;

	private:
		HRESULT m_Hr;
	};


	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr);
		const char* what() const override;
		const char* GetType() const override;
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
		std::string GetErrorDescription() const;

	private:
		HRESULT m_Hr;
	};

	class DeviceRemovedException : public HrException
	{
	public:
		DeviceRemovedException(int line, const char* file, HRESULT hr);
		const char* GetType() const override;
	};


	class NoGraphicsException : public Exception
	{
	public:
		NoGraphicsException(int line, const char* file);
		const char* GetType() const;
	};

}


#define THWND_EXCEPT(hr) Except::WindowException(__LINE__, __FILE__, hr);
#define THWND_LAST_EXCEPT() Except::WindowException(__LINE__, __FILE__, GetLastError());

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Except::HrException(__LINE__, __FILE__, hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Except::DeviceRemovedException(__LINE__, __FILE__, hr)

#define THWND_NO_GFX_EXCEPT() Except::NoGraphicsException(__LINE__, __FILE__)