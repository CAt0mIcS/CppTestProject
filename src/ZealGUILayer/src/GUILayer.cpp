#include "glpch.h"
#include "GUILayer.h"

// You need to include this to use the logger
#include <RlDebug/ReyalLogger.h>

#include <RlDebug/Instrumentor.h>


namespace At0::Layers
{
	GUILayer::GUILayer(const std::wstring_view name)
		: _In_ Reyal::Layer(name)
	{
		RL_PROFILE_FUNCTION();
	}

	void GUILayer::OnMouseMove(_In_ Reyal::Widget* receiver, Reyal::MouseMoveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	void GUILayer::OnMousePress(_In_ Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseRelease(_In_ Reyal::Widget* receiver, Reyal::MouseButtonReleasedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseWheelUp(_In_ Reyal::Widget* receiver, Reyal::MouseWheelUpEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnMouseWheelDown(_In_ Reyal::Widget* receiver, Reyal::MouseWheelDownEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnResize(_In_ Reyal::Widget* receiver, Reyal::WindowResizeEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnWindowMove(_In_ Reyal::Widget* receiver, Reyal::WindowMoveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnPaint(_In_ Reyal::Widget* receiver, Reyal::PaintEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverEnter(_In_ Reyal::Widget* receiver, Reyal::HoverEnterEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnHoverLeave(_In_ Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnKeyPress(_In_ Reyal::Widget* receiver, Reyal::KeyPressedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnKeyRelease(_In_ Reyal::Widget* receiver, Reyal::KeyReleasedEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}

	void GUILayer::OnChar(_In_ Reyal::Widget* receiver, Reyal::CharEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
	}
	
	bool GUILayer::OnWindowClose(_In_ Reyal::Widget* receiver, Reyal::WindowCloseEvent& e)
	{
		RL_PROFILE_FUNCTION();
		RL_LOG_DEBUG("[GUILayer] {0}", e.ToString());
		return false;
	}
}
