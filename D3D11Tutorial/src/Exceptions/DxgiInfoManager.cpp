#include "pch.h"
#include "DxgiInfoManager.h"

#include "Exception.h"
#include <dxgidebug.h>


#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Except::HrException(__LINE__, __FILE__, hrcall)


DxgiInfoManager::DxgiInfoManager()
{
	//define function signature for DXGIGetDebugInterface
	typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	//load dll containing function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
	if (!hModDxgiDebug)
		throw THWND_LAST_EXCEPT();

	//get address of DXGIGetDebugInterface function in dll
	const auto DxgiGetDebugInterface = (DXGIGetDebugInterface)(void*)GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface");
	if(!DxgiGetDebugInterface)
		throw THWND_LAST_EXCEPT();

	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)m_pDxgiInfoQueue));

}

void DxgiInfoManager::Set()
{
	//sets the index (next) so that the next call to GetMessages() 
	//will only get errors generated after this call
	m_Next = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	
	for (auto i = m_Next; i < end; ++i)
	{
		HRESULT hr;
		SIZE_T messageLength;

		//get size of message i in bytes
		GFX_THROW_NOINFO(m_pDxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		
		//allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = (DXGI_INFO_QUEUE_MESSAGE*)bytes.get();

		//get the message and push it's description into the vector
		GFX_THROW_NOINFO(m_pDxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}
