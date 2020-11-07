#ifndef ZGL_GUILAYER_H
#define ZGL_GUILAYER_H

#include "glbase.h"

#include <Reyal/Reyal.h>


namespace At0::Layers
{
	class ZGL_API GUILayer : public Reyal::Layer
	{
	public:
		GUILayer(const std::wstring_view name);

		virtual void OnMousePress(_In_ Reyal::Widget* receiver, Reyal::MouseButtonPressedEvent& e) override;
		virtual void OnMouseRelease(_In_ Reyal::Widget* receiver, Reyal::MouseButtonReleasedEvent& e) override;
		virtual void OnMouseWheelUp(_In_ Reyal::Widget* receiver, Reyal::MouseWheelUpEvent& e) override;
		virtual void OnMouseWheelDown(_In_ Reyal::Widget* receiver, Reyal::MouseWheelDownEvent& e) override;
		virtual void OnMouseMove(_In_ Reyal::Widget* receiver, Reyal::MouseMoveEvent& e) override;
		virtual void OnHoverEnter(_In_ Reyal::Widget* receiver, Reyal::HoverEnterEvent& e) override;
		virtual void OnHoverLeave(_In_ Reyal::Widget* receiver, Reyal::HoverLeaveEvent& e) override;
		virtual void OnKeyPress(_In_ Reyal::Widget* receiver, Reyal::KeyPressedEvent& e) override;
		virtual void OnKeyRelease(_In_ Reyal::Widget* receiver, Reyal::KeyReleasedEvent& e) override;
		virtual void OnChar(_In_ Reyal::Widget* receiver, Reyal::CharEvent& e) override;
		virtual void OnResize(_In_ Reyal::Widget* receiver, Reyal::WindowResizeEvent& e) override;
		virtual void OnWindowMove(_In_ Reyal::Widget* receiver, Reyal::WindowMoveEvent& e) override;
		virtual void OnPaint(_In_ Reyal::Widget* receiver, Reyal::PaintEvent& e) override;
		virtual bool OnWindowClose(_In_ Reyal::Widget* receiver, Reyal::WindowCloseEvent& e) override;
	};
}

extern "C"
{
	ZGL_API At0::Layers::GUILayer* CreateLayer()
	{
		return new At0::Layers::GUILayer(L"GUI-Layer");
	}
}

#endif // ZGL_GUILAYER_H
