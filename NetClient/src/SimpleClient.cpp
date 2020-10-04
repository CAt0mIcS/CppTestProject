#include <iostream>
#include <Net.h>


enum class CustomMsgTypes : uint32_t
{
	FireBullet,
	MovePlayer
};


int main()
{
	net::Message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::FireBullet;

	float x = 2.0f;
	bool t = true;

	msg << x << t;

	x = 4.0f;
	t = false;

	msg >> t >> x;

}
