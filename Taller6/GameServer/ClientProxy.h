#pragma once
#include <SFML\Network.hpp>
#include "GeneralInfo.h"

using namespace sf;
using namespace std;

struct POSITION
{
	int x, y;
};

class ClientProxy {
private:
	vector<Packet>unrespondedMsgs;	
public:
	POSITION position;
	IpAddress ip;	
	unsigned short port;
	string nick;
	ClientProxy(IpAddress myIp, unsigned short myPort, POSITION pos, string n);
	bool operator==(const ClientProxy& other);
	void ResendMsgs(UdpSocket* sock);
	void CheckACK(int id);
};
