#pragma once

#ifdef TOOLS_BUILD
#define TL_API __declspec(dllexport)
#else
#define TL_API __declspec(dllimport)
#endif



class TL_API ToolBase
{
public:
	ToolBase();

	bool ShouldClose() const { return m_ShouldClose; }

	virtual void ToolDo() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnClose() = 0;

protected:
	bool m_ShouldClose = false;
};

