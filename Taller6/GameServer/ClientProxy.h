#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include "GeneralInfo.h"

#define RESEND_TIME 500

using namespace sf;
using namespace std;

struct POSITION
{
	int x, y;
};

class ClientProxy {
private:
	vector<Packet>unrespondedMsgs;
	Clock resendClock;
public:
	POSITION position;
	IpAddress ip;	
	unsigned short port;
	string nick;
	ClientProxy(IpAddress myIp, unsigned short myPort, POSITION pos, string n);
	bool operator==(const ClientProxy& other);
	void ResendMsgs(UdpSocket* sock);
	void MesageResponded(int id);
};
