#pragma once

#include "../Tools/src/ToolBase.h"

#ifdef CM2_BUILD
#define CM2_API __declspec(dllexport)
#else
#define CM2_API __declspec(dllimport)
#endif


class CM2_API ToolDerived2 : public ToolBase
{
public:
	ToolDerived2() = default;

	virtual void ToolDo() override;
	virtual void OnUpdate() override;
	virtual void OnClose() override;
};


extern "C" CM2_API ToolBase* CreateTool()
{
	return new ToolDerived2();
}

