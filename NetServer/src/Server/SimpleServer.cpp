#include <NetCommon/Net.h>


enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage
};


class CustomServer : public Net::ServerInterface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t port)
		: Net::ServerInterface<CustomMsgTypes>(port)
	{
		
	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<Net::Connection<CustomMsgTypes>> client) override
	{
		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<Net::Connection<CustomMsgTypes>> client) override
	{

	}

	virtual void OnMessage(std::shared_ptr<Net::Connection<CustomMsgTypes>> client, Net::Message<CustomMsgTypes>& message) override
	{
		switch (message.header.id)
		{
		case CustomMsgTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "] Server Ping\n";

			client->Send(message);
			break;
		}
		}
	}
};


int main()
{
	CustomServer server(60000);
	server.Start();

	while (true)
	{
		server.Update();
	}

	return 0;
}

