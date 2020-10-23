#pragma once

#include "ToolBase.h"

#ifdef CM_BUILD
#define CM_API __declspec(dllexport)
#else
#define CM_API __declspec(dllimport)
#endif



class CM_API ToolDerived : public ToolBase
{
public:
	ToolDerived();

	virtual void ToolDo() override;
	virtual void OnUpdate() override;
	virtual void OnClose() override;
};



extern "C" CM_API ToolBase* CreateTool();