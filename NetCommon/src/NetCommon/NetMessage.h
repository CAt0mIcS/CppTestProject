#pragma once
#include "NetCommon.h"


namespace net
{
	template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
	struct MessageHeader
	{
		T id{};
		uint32_t size;
	};

	template<typename T>
	struct Message
	{
		MessageHeader<T> header{};
		std::vector<uint8_t> body;

		/// <summary>
		/// Calculates the size of the Message
		/// </summary>
		/// <returns>The size of the message</returns>
		size_t Size() const
		{
			return sizeof(MessageHeader<T>) + body.size();
		}

		/// <summary>
		/// Overloaded operator for std::cout
		/// When calling std::cout on a message, this function will be called
		/// and will print the message id and size
		/// </summary>
		/// <param name="os">Specifies the output stream</param>
		/// <param name="message">Specifies the message to output</param>
		/// <returns>The output stream</returns>
		friend std::ostream& operator <<(std::ostream& os, const Message<T>& message)
		{
			os << "ID: " << (int)message.header.id << " Size: " << message.Size();
			return os;
		}

		/// <summary>
		/// Operator to push data into the message body
		/// </summary>
		/// <typeparam name="DataType">Is the type of the second function argument, can be any standard layout data type</typeparam>
		/// <param name="msg">Is the message where the data will be pushed into</param>
		/// <param name="data">is the data which will be pushed into the message body</param>
		/// <returns>A reference to the message so it can be "chained"</returns>
		template<typename DataType>
		friend Message<T>& operator <<(Message<T>& msg, const DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed");

			size_t i = msg.body.size();
			msg.body.resize(msg.body.size() + sizeof(DataType));

			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
			
			msg.header.size = msg.Size();
			return msg;
		}

		/// <summary>
		/// Operator to pull data from the message into the "data" function argument
		/// </summary>
		/// <typeparam name="DataType">Is the type of the data to pull from the message</typeparam>
		/// <param name="msg">Is the message where the data will be pulled from</param>
		/// <param name="data">Is the data where the function will push the value into</param>
		/// <returns>A reference to the message so it can be "chained"</returns>
		template<typename DataType>
		friend Message<T>& operator >>(Message<T>& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled");
			
			size_t i = msg.body.size() - sizeof(DataType);

			std::memcpy(&data, msg.body.data() + i, sizeof(DataType));
			
			msg.body.resize(i);
			msg.header.size = msg.Size();
			
			return msg;
		}
	};


	template<typename T>
	class Connection;

	template<typename T>
	struct OwnedMessage
	{
		std::shared_ptr<Connection<T>> remote = nullptr;
		Message<T> msg;

		/// <summary>
		/// Overloaded operator for std::cout
		/// When calling std::cout on a message, this function will be called
		/// and will print the message id and size
		/// </summary>
		/// <param name="os">Specifies the output stream</param>
		/// <param name="msg">Specifies the message to output</param>
		/// <returns>The output stream</returns>
		friend std::ostream& operator <<(std::ostream& os, const OwnedMessage<T>& msg)
		{
			os << msg.msg;
			return os;
		}

	};
}
