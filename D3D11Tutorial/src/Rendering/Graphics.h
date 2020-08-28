#pragma once

#include "pch.h"
#include "Exceptions/Exception.h"

#include <d3d11.h>


class Graphics
{
public:
	class Exception : public Except::Exception
	{
		//using Except::Exception::Exception;
	};
	
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file);
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
		const char* GetType() const override;
	};

public:
	Graphics(HWND hWnd);

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	/**
	* Switches back and front buffer
	*/
	void EndFrame();

	/*
	* Clears the back buffer
	*/
	void ClearBuffer(float red, float green, float blue);

	~Graphics();

private:
	ID3D11Device* m_pDevice;
	IDXGISwapChain* m_pSwapChain;
	ID3D11DeviceContext* m_pContext;
	ID3D11RenderTargetView* m_pTarget;
};

