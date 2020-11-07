#include "rlpch.h"
#include "Application.h"

#include "Layers/Layer.h"

#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

#include <RlDebug/RlAssert.h>
#include <RlDebug/Instrumentor.h>
#include <RlDebug/ReyalLogger.h>


namespace At0::Reyal
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_MainWindow(L"MainWindow", nullptr, true), m_LayerStack{}, m_Running(true)
	{
		m_MainWindow.SetImmediateEventHandler([this](_In_ Widget* receiver, Event& e) { return OnImmediateEvent(receiver, e); });
		RL_LOG_INFO("[ThreadPool] Initialized {0} threads", m_ThreadPool.MaxThreads());
	}

	void Application::Create(_In_ Application* app)
	{
		s_Instance = app;
	}

	void Application::Destroy()
	{
		delete s_Instance;
	}

	int Application::Run()
	{
		//////////////////////////////////////////////////////////////////////////////////////////
		////////// Event Handling Loop ///////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		auto dispatchEvents = [this]()
		{
			auto& queue = m_MainWindow.GetEventQueue();
			while (m_Running)
			{
				EventMessage eMsg = queue.PopFront();
				// Dispatch the Event to the layers
				OnEventReceived(eMsg.receiver, std::move(eMsg.e));
				// Wait for new events to come in or the program to exit
				queue.WaitFor([&queue, this]() { return !queue.Empty() || !m_Running; });
			}
		};
		m_ThreadPool.AddTask(dispatchEvents);


		//////////////////////////////////////////////////////////////////////////////////////////
		////////// Main Application Loop /////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		int exitCode = 0;
		while (!m_MainWindow.ShouldClose(&exitCode))
		{
			for (auto* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
		}

		m_Running = false;
		
		// We need to notify all threads that we've exited the application
		// and that they should stop waiting, we could also push a dummy event 
		// into the queue
		m_MainWindow.GetEventQueue().GetWaiter().notify_all();

		return exitCode;
	}

	void Application::PushLayer(_In_ Layer* layer)
	{
		RL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
	}

	Application::~Application()
	{
		RL_PROFILE_FUNCTION();
	}
	
	void Application::OnEventReceived(_In_ Widget* receiver, Scope<Event>&& e)
	{
		RL_PROFILE_FUNCTION();

		// Dispatch event to every layer
		for (auto* layer : m_LayerStack)
		{
			if (e->Handled)
				break;
			DispatchEvent(layer, receiver, *e);
		}
	}

	bool Application::OnImmediateEvent(_In_ Widget* receiver, Event& e)
	{
		RL_PROFILE_FUNCTION();
		RL_EXPECTS(e.GetType() <= EventType::LAST && e.GetType() >= EventType::FIRST);

		// Wait for queue to be empty
		auto& queue = m_MainWindow.GetEventQueue();
		queue.WaitFor([&queue]() { return queue.Empty(); });

		for (auto* layer : m_LayerStack)
		{
			switch (e.GetType())
			{
			case EventType::WindowCloseEvent:			return layer->OnWindowClose(receiver, (WindowCloseEvent&)e);
			default:
				RL_ASSERT(false, "Unimplemented Event");
			}
		}
		return false;
	}
	
	void Application::DispatchEvent(_In_ Layer* layer, _In_ Widget* receiver, Event& e)
	{
		RL_PROFILE_FUNCTION();

		RL_EXPECTS(e.GetType() <= EventType::LAST && e.GetType() >= EventType::FIRST);

		switch (e.GetType())
		{
		case EventType::MouseButtonPressedEvent:	layer->OnMousePress(receiver, (MouseButtonPressedEvent&)e); break;
		case EventType::MouseButtonReleasedEvent:	layer->OnMouseRelease(receiver, (MouseButtonReleasedEvent&)e); break;
		case EventType::MouseWheelUpEvent:			layer->OnMouseWheelUp(receiver, (MouseWheelUpEvent&)e); break;
		case EventType::MouseWheelDownEvent:		layer->OnMouseWheelDown(receiver, (MouseWheelDownEvent&)e); break;
		case EventType::MouseMoveEvent:				layer->OnMouseMove(receiver, (MouseMoveEvent&)e); break;
		case EventType::HoverEnterEvent:			layer->OnHoverEnter(receiver, (HoverEnterEvent&)e); break;
		case EventType::HoverLeaveEvent:			layer->OnHoverLeave(receiver, (HoverLeaveEvent&)e); break;
		case EventType::KeyPressedEvent:			layer->OnKeyPress(receiver, (KeyPressedEvent&)e); break;
		case EventType::KeyReleasedEvent:			layer->OnKeyRelease(receiver, (KeyReleasedEvent&)e); break;
		case EventType::CharEvent:					layer->OnChar(receiver, (CharEvent&)e); break;
		case EventType::WindowResizeEvent:			layer->OnResize(receiver, (WindowResizeEvent&)e); break;
		case EventType::WindowMoveEvent:			layer->OnWindowMove(receiver, (WindowMoveEvent&)e); break;
		case EventType::PaintEvent:					layer->OnPaint(receiver, (PaintEvent&)e); break;
		//case EventType::SetCursorEvent:			layer->OnSetCursor(receiver, (SetCursorEvent&)e); break;
		default:
			RL_ASSERT(false, "Unimplemented Event");
		}
	}
}


