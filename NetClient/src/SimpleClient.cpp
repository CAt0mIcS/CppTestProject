#include <iostream>
#include <NetCommon/Net.h>


enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage
};



class CustomClient : public Net::ClientInterface<CustomMsgTypes>
{
public:
	void PingServer()
	{
		Net::Message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		// Different implementations on different PCs (not usable for actual server)
		auto timeNow = std::chrono::system_clock::now();
		
		msg << timeNow;
		Send(msg);
	}

};


int main()
{
	CustomClient c;
	c.Connect("127.0.0.1", 60000);

	bool key[3] = { false, false, false };
	bool oldKey[3] = { false, false, false };

	bool quit = false;
	while (!quit)
	{
		if (GetForegroundWindow() == GetConsoleWindow())
		{
			key[0] = GetAsyncKeyState('1') & 0x8000;
			key[1] = GetAsyncKeyState('2') & 0x8000;
			key[2] = GetAsyncKeyState('3') & 0x8000;
		}

		if (key[0] && !oldKey[0])
			c.PingServer();
		//if(key[1] && !oldKey[1])

		if (key[2] && !oldKey[2])
			quit = true;

		for (int i = 0; i < 3; ++i)
			oldKey[i] = key[i];

		if (c.IsConnected())
		{
			if (!c.Incoming().Empty())
			{
				auto msg = c.Incoming().PopFront().msg;

				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerPing:
				{
					auto timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;

					std::cout << "Ping: " << round(std::chrono::duration<double>(timeNow - timeThen).count()) << '\n';
				}
				}
			}
		}

	}

}
