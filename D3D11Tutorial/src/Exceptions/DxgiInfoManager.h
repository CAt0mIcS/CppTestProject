#pragma once

#include "pch.h"
#include <wrl.h>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

	void Set();
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long m_Next = 0u;
	Microsoft::WRL::ComPtr<struct IDXGIInfoQueue> m_pDxgiInfoQueue = nullptr;
};
