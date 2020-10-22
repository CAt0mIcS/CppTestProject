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

	virtual void ToolDo() {};

	int i = 0;
};

