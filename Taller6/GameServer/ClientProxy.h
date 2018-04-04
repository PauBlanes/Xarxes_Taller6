#pragma once
#include <SFML\Network.hpp>

using namespace sf;

class ClientProxy {
private:
	
public:
	IpAddress ip;	
	unsigned short port;
	ClientProxy(IpAddress myIp, unsigned short myPort);
	bool operator==(const ClientProxy& other);
};
