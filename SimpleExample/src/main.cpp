#include "pch.h"

static std::vector<char> g_Buffer(20 * 1024);


void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(g_Buffer.data(), g_Buffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for (uint32_t i = 0; i < length; ++i)
					std::cout << g_Buffer[i];

				GrabSomeData(socket);
			}
		}
	);
}


//Tutorial: https://www.youtube.com/watch?v=2hNdkYInj4g
int main(int argc, char** argv)
{
	asio::error_code ec;
	asio::io_context context;

	asio::io_context::work idleWork(context);

	std::thread thrContext([&]() { context.run(); });

	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);
	asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, ec);
	if (!ec)
		std::cout << "Connected\n";
	else
		std::cout << "Failed to connect to address:\n" << ec.message() << '\n';

	if (socket.is_open())
	{
		GrabSomeData(socket);

		std::string request =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(request.data(), request.size()), ec);

		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(20000ms);
		}

		context.stop();
		if (thrContext.joinable())
			thrContext.join();
	}
	
}

