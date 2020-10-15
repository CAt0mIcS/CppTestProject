#pragma once

#include "NetCommon.h"
#include "NetMessage.h"
#include "ts/NetQueue.h"
#include "NetConnection.h"


namespace Net
{
	template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
	class ClientInterface
	{
	public:
		/// <summary>
		/// Default ClientInterface constructor
		/// </summary>
		ClientInterface()
			: m_Socket(m_Context)
		{

		}

		/// <summary>
		/// ClientInterface Deconstructor, automatically disconnects from the server
		/// </summary>
		virtual ~ClientInterface()
		{
			Disconnect();
		}

		/// <summary>
		/// Connects to the server with hostname/ip-address and port
		/// </summary>
		/// <param name="host">Is the host name</param>
		/// <param name="port">Is the port</param>
		/// <returns>True if the connection was established successfully, false otherwise</returns>
		bool Connect(const std::string& host, const uint16_t port)
		{
			try
			{
				asio::ip::tcp::resolver resolver(m_Context);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
				
				m_Connection = std::make_unique<Connection<T>>(
					Connection<T>::Owner::Client,
					m_Context,
					asio::ip::tcp::socket(m_Context),
					m_qMessagesIn
				);

				m_Connection->ConnectToServer(endpoints);

				m_thrContext = std::thread([this]() { m_Context.run(); });
			}
			catch (std::exception& e)
			{
				std::cerr << "Client Exception: " << e.what() << '\n';
			}
			return false;
		}

		/// <summary>
		/// Disconnects from the server
		/// </summary>
		void Disconnect()
		{
			if (IsConnected())
			{
				m_Connection->Disconnect();
			}

			m_Context.stop();
			if (m_thrContext.joinable())
				m_thrContext.join();
		}

		/// <summary>
		/// Checks if we have a valid connection
		/// </summary>
		/// <returns>True if the socket is connected to the server, false otherwise</returns>
		bool IsConnected() const
		{
			if (m_Connection)
				return m_Connection->IsConnected();
			return false;
		}

		/// <summary>
		/// Getter for thread save queue with owned messages
		/// </summary>
		/// <returns>A reference to the thread save queue</returns>
		TS::Queue<OwnedMessage<T>>& Incoming()
		{
			return m_qMessagesIn;
		}

		/// <summary>
		/// Sends a message
		/// </summary>
		/// <param name="msg">Is the message which will be sent</param>
		/// <returns>True if the message was sent successfully, false otherwise</returns>
		void Send(const Message<T>& msg)
		{
			m_Connection->Send(msg);
		}

	protected:
		/// <summary>
		/// ASIO Context handles the data transfer
		/// </summary>
		asio::io_context m_Context;

		/// <summary>
		/// Context needs a thread of its own to execute its work commands
		/// </summary>
		std::thread m_thrContext;

		/// <summary>
		/// This is the hardware socket that is connected to the server
		/// </summary>
		asio::ip::tcp::socket m_Socket;

		/// <summary>
		/// The client has a single instance of a "Connection" object, which handles data transfer
		/// </summary>
		std::unique_ptr<Connection<T>> m_Connection;

	private:
		/// <summary>
		/// This is the thread safe queue of incomming messages from the server
		/// </summary>
		TS::Queue<OwnedMessage<T>> m_qMessagesIn;
	};
}
