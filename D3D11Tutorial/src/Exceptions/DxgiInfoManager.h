#pragma once

#include "pch.h"


class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

	void Set();
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long m_Next = 0u;
	struct IDXGIInfoQueue* m_pDxgiInfoQueue = nullptr;
};
