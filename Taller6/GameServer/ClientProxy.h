#pragma once
#include <SFML\Network.hpp>

using namespace sf;

struct POSITION
{
	int x, y;
};

class ClientProxy {
private:
	
public:
	POSITION position;
	IpAddress ip;	
	unsigned short port;
	ClientProxy(IpAddress myIp, unsigned short myPort, POSITION pos);
	bool operator==(const ClientProxy& other);
};
