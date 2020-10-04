#include <iostream>
#include <NetCommon/Net.h>


enum class CustomMsgTypes : uint32_t
{
	FireBullet,
	MovePlayer
};



class CustomClient : public net::ClientInterface<CustomMsgTypes>
{
public:
	bool FireBullet(float x, float y)
	{
		net::Message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::FireBullet;
		msg << x << y;
		return Send(msg);
	}
};


int main()
{
	CustomClient c;
	c.Connect("", 0);
	c.FireBullet(2.0f, 5.0f);
}
