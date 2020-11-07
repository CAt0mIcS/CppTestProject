#ifndef ZL_SANDBOX_H
#define ZL_SANDBOX_H

#include <Reyal/Application.h>

namespace At0
{

	class Sandbox : public Reyal::Application
	{
	public:
		/// <summary>
		/// Sandbox Constructor
		/// </summary>
		Sandbox();

		/// <summary>
		/// Sandbox Deconstructor
		/// </summary>
		~Sandbox();

		/// <summary>
		/// Receives all window events (WindowResize, WindowClose, WindowPaint)
		/// </summary>
		/// <param name="receiver">Is the window which receives the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		virtual void OnEvent(Reyal::Widget* receiver, Reyal::Event& e);

	};


	void Reyal::CreateApplication()
	{
		Reyal::Application::Create(new Sandbox());
	}
}


#endif // ZL_SANDBOX_H
