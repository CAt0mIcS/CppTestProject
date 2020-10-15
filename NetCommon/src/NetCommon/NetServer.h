#pragma once

#include "NetCommon.h"
#include "ts/NetQueue.h"
#include "NetMessage.h"
#include "NetConnection.h"


namespace Net
{
	template<typename T>
	class ServerInterface
	{
	public:
		ServerInterface(uint16_t port)
			: m_AsioAcceptor(m_AsioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{

		}

		virtual ~ServerInterface()
		{
			Stop();
		}

		/// <summary>
		/// Starts the server
		/// </summary>
		/// <returns>True if the server started successfully, false otherwise</returns>
		bool Start()
		{
			try
			{
				WaitForClientConnection();
				m_ThreadContext = std::thread([this]() { m_AsioContext.run(); });
			}
			catch (std::exception& e)
			{
				// Something prohibited the server from listening
				std::cerr << "[SERVER] Exception: " << e.what() << '\n';
				return false;
			}

			std::cout << "[SERVER] Started!\n";
			return true;
		}

		/// <summary>
		/// Stops the server
		/// </summary>
		void Stop()
		{
			// Request the context to close
			m_AsioContext.stop();

			// Tidy up the context thread
			if (m_ThreadContext.joinable())
				m_ThreadContext.join();

			std::cout << "[SERVER] Stopped!\n";
		}

		/// <summary>
		/// ASYNC - Instruct asio to wait for connection
		/// </summary>
		void WaitForClientConnection()
		{
			m_AsioAcceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					if (!ec)
					{
						std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << '\n';

						std::shared_ptr<Connection<T>> newConn = 
							std::make_shared<Connection<T>>(Connection<T>::Owner::Server, 
								m_AsioContext, std::move(socket), m_MessagesIn);

						// Give the user server a chance to deny connection
						if (OnClientConnect(newConn))
						{
							// Connection allowed, so add to container of new connections
							m_Connections.push_back(std::move(newConn));

							m_Connections.back()->ConnectToClient(m_IDCounter++);

							std::cout << '[' << m_Connections.back()->GetID() << "] Connection Approved\n";
						}
						else
						{
							std::cout << "[------] Connection Denied\n";
						}
					}
					else
					{
						// Error has occured during acceptance
						std::cerr << "[SERVER] New Connection Error: " << ec.message() << '\n';
					}

					// Prime the asio context with more work
					WaitForClientConnection();
				}
			);
		}

		/// <summary>
		/// Sends a message to a specific client
		/// </summary>
		/// <param name="client">Specifies the client to send the message to</param>
		/// <param name="msg">Specifies the message to send to the client</param>
		void MessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg)
		{
			if (client && client->IsConnected())
			{
				client->Send(msg);
			}
			else
			{
				OnClientDisconnect(client);
				client.reset();
				m_Connections.erase(
					std::remove(m_Connections.begin(), m_Connections.end(), client), m_Connections.end()
				);
			}
		}

		/// <summary>
		/// Sends a message to all clients
		/// </summary>
		/// <param name="msg">Specifies the message to send to the clients</param>
		/// <param name="pIgnoreClient"></param>
		void MessageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr)
		{
			bool invalidClientExists = false;

			for (auto& client : m_Connections)
			{
				if (client && client->IsConnected())
				{
					if (client != pIgnoreClient)
						client->Send(msg);
				}
				else
				{
					// The client couldn't be contacted, so assume it's disconnected
					OnClientDisconnect(client);
					client.reset();
					invalidClientExists = true;
				}
			}

			if (invalidClientExists)
			{
				m_Connections.erase(
					std::remove(m_Connections.begin(), m_Connections.end(), nullptr), m_Connections.end()
				);
			}

		}

		/// <summary>
		/// Called by the user to explicitly process some of the messages in the queue
		/// </summary>
		/// <param name="maxMessages">Is the maximum number of messages to process</param>
		void Update(size_t maxMessages = -1)
		{
			size_t messageCount = 0;
			while (messageCount < maxMessages && !m_MessagesIn.Empty())
			{
				// Grab the front message
				auto msg = m_MessagesIn.PopFront();

				// Pass the message handler
				OnMessage(msg.remote, msg.msg);
				++messageCount;
			}
		}

	protected:
		/// <summary>
		/// Called when a client connects, you can deny the connection by returning false
		/// </summary>
		/// <param name="client">Specifies the client that connected</param>
		/// <returns>True if the client should connect, false otherwise</returns>
		virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client)
		{
			return false;
		}

		/// <summary>
		/// Called when a client appears to have disconnected
		/// </summary>
		/// <param name="client">Specifies the client that disconnected</param>
		virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client)
		{

		}

		/// <summary>
		/// Called when a message arrives
		/// </summary>
		/// <param name="client">Specifies the client that sent the message</param>
		/// <param name="msg">Specifies the message sent</param>
		virtual void OnMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg)
		{

		}

	protected:
		/// <summary>
		/// Thread Safe Queue for incoming message packets
		/// </summary>
		TS::Queue<OwnedMessage<T>> m_MessagesIn;

		/// <summary>
		/// Container of active validated connections
		/// </summary>
		std::deque<std::shared_ptr<Connection<T>>> m_Connections;

		/// <summary>
		/// Asio Context
		/// </summary>
		asio::io_context m_AsioContext;
		
		/// <summary>
		/// Thread to assign to asio context for idle work
		/// </summary>
		std::thread m_ThreadContext;

		/// <summary>
		/// Asio Acceptor
		/// </summary>
		asio::ip::tcp::acceptor m_AsioAcceptor;

		/// <summary>
		/// Clients will be identified via an ID
		/// </summary>
		uint32_t m_IDCounter = 10000;
	};
}
