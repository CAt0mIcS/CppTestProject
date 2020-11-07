#include "pch.h"
#include "Sandbox.h"


//QUESTION: When entry point is included in Reyal.h
#include <Reyal/Reyal.h>
#include <RlDebug/Instrumentor.h>

#include <filesystem>

#include <Windows.h>

namespace At0
{
	Sandbox::Sandbox()
	{
		RL_PROFILE_FUNCTION();

		m_MainWindow.InitRenderer3D();
		m_MainWindow.SetTitle(L"Zeal");
		m_MainWindow.Show();

		// Test loading in layers
		//using LayerCreateFunc = Reyal::Layer* (*)();
		//const std::wstring ending = L".dll";

		//for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(ZL_OUT_DIR + std::string("/Editors")))
		//{
		//	std::wstring_view path = dirEntry.path().native();

		//	if (path.size() < ending.size() || path.compare(path.size() - ending.size(), ending.size(), ending) != 0)
		//		continue;

		//	HMODULE hDll = LoadLibrary(path.data());
		//	if (!hDll || hDll == INVALID_HANDLE_VALUE)
		//	{
		//		MessageBox(NULL, L"Error", L"Cannot find dll", MB_OK);
		//		return;
		//	}

		//	LayerCreateFunc layerCreateFunc = (LayerCreateFunc)GetProcAddress(hDll, "CreateLayer");
		//	if (!layerCreateFunc)
		//	{
		//		MessageBox(NULL, L"Error", L"Cannot find layer create function", MB_OK);
		//		return;
		//	}

		//	PushLayer(layerCreateFunc());
		//}

	}
	
	Sandbox::~Sandbox()
	{
		RL_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Reyal::Widget* receiver, Reyal::Event& e)
	{
		RL_PROFILE_FUNCTION();

	}

}
