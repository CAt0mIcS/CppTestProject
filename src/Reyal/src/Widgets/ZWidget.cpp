#include "rlpch.h"
#include "ZWidget.h"

#include "Input/Mouse.h"

#include <RlDebug/Instrumentor.h>
#include <RlUtil/Exception.h>
#include <RlUtil/TypeConvert.h>


namespace At0::Reyal
{
    void Widget::MoveBy(const Point2& pos)
    {
        RL_PROFILE_FUNCTION();

    }

    void Widget::MoveTo(const Point2& pos)
    {
        RL_PROFILE_FUNCTION();

    }

    void Widget::ResizeBy(const Size& size)
    {
        RL_PROFILE_FUNCTION();

    }

    void Widget::ResizeTo(const Size& size)
    {
        RL_PROFILE_FUNCTION();

    }

    Widget* Widget::FindChild(const std::wstring_view name)
    {
        RL_PROFILE_FUNCTION();
        for (auto& child : m_Children)
        {
            if (child->GetName() == name)
                return child.get();
        }
        return nullptr;
    }

    void Widget::PostRedraw(bool eraseBkgnd) const
    {
        HWND hWnd = GetNativeWindow();
        
        //TODO: Program should be able to continue if one call fails

        //TODO: Invalidate only widget area
        RL_THROW_LAST_WND_EXCEPT(InvalidateRect(hWnd, &Util::TransformationToRect(m_Matrix), (BOOL)eraseBkgnd));
        RL_THROW_LAST_WND_EXCEPT(UpdateWindow(hWnd));
    }

    /// <summary>
    /// Defines commonly used operators
    /// </summary>
    #pragma region Operators

    bool Widget::operator==(const Widget& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    bool Widget::operator==(const Scope<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator==(const Ref<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator!=(const Widget& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other.GetName();
    }

    bool Widget::operator!=(const Scope<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }

    bool Widget::operator!=(const Ref<Widget>& other)
    {
        RL_PROFILE_FUNCTION();
        return GetName() == other->GetName();
    }
    
    #pragma endregion

    Widget::~Widget()
    {
        RL_PROFILE_FUNCTION();
    }

    Widget::Widget(const std::wstring_view name, _In_opt_ Widget* parent)
        : m_Name(name), m_Parent(parent), m_Children{}, m_Matrix{}
    {
        RL_PROFILE_FUNCTION();
    }

    Widget* Widget::GetEventReceiver(const Event& e, const Mouse& mouse)
    {
        RL_PROFILE_FUNCTION();

        Widget* receiver = nullptr;

        //The main window should always receive the event if no other receiver is found
        if (!GetParent())
            receiver = this;

        if (mouse.IsOnWidget(this))
        {
            for (auto& child : m_Children)
            {
                Widget* receiver2 = child->GetEventReceiver(e, mouse);
                if (receiver2)
                {
                    receiver = receiver2;
                    break;
                }
            }

            if (!receiver)
                receiver = this;
        }

        return receiver;
    }
}

