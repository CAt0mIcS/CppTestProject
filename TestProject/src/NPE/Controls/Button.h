#pragma once

#include "pch.h"
#include "NPE/Renderer/Renderer2D.h"

class Button
{
public:
	Button(Renderer2D& renderer, float x, float y, float width, float height, const std::wstring& text)
		: m_X(x), m_Y(y), m_Width(width), m_Height(height), Renderer(renderer), m_Text(text)
	{
	}

	/**
	* @param x is mouse pos x
	* @param y is mouses pos y
	*/
	bool IsMouseOnButton(float x, float y)
	{
		return x > m_X && x < m_X + m_Width && y > m_Y && y < m_Y + m_Height;
	}

	void Paint()
	{
		Renderer.DrawTestButton(m_X, m_Y, m_Width, m_Height);
		Renderer.DrawString(m_X, m_Y, 50, m_Text.c_str());
	}

	void OnClick()
	{
		MessageBox(NULL, L"Hello World", L"", NULL);
	}

private:
	float m_X, m_Y, m_Width, m_Height;
	Renderer2D& Renderer;
	std::wstring m_Text;
};
