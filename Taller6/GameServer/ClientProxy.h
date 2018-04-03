#pragma once
#include <SFML\Network.hpp>

using namespace sf;

class ClientProxy {
private:
	IpAddress ip;
	unsigned short port;
public:
	ClientProxy(IpAddress myIp, unsigned short myPort);
};
