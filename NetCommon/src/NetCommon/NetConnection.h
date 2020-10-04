#pragma once

#include "NetCommon.h"
#include "ts/NetQueue.h"
#include "NetMessage.h"


namespace net
{
	template<typename T>
	class Connection : public std::enable_shared_from_this<Connection<T>>
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		Connection()
		{

		}

		/// <summary>
		/// 
		/// </summary>
		~Connection()
		{
		
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool ConnectToServer()
		{
			return false;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool Disconnect()
		{
			return false;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsConnected() const
		{
			return false;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="msg"></param>
		/// <returns></returns>
		bool Send(const Message<T>& msg)
		{
			return false;
		}


	protected:
		asio::ip::tcp::socket m_Socket;
		asio::io_context m_asioContext;
		ts::Queue<Message<T>> m_qMessagesOut;
		ts::Queue<OwnedMessage<T>>& m_qMessagesIn;
	};
}
