#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include "GeneralInfo.h"
#include <time.h> 

#define RESEND_TIME 500
#define PERCENT_LOSS 25

using namespace sf;
using namespace std;

struct POSITION
{
	int x, y;
};

struct BufferAndLength {
	char* buffer;
	int length;
};

class ClientProxy {
private:	
	Clock resendClock;
public:
	//vector<Packet>unrespondedMsgs;
	map<int, BufferAndLength> msgs2Resend;
	POSITION position;
	IpAddress ip;	
	unsigned short port;
	string nick;
	ClientProxy(IpAddress myIp, unsigned short myPort, POSITION pos, string n);
	bool operator==(const ClientProxy& other);
	void ResendMsgs(UdpSocket* sock);
	void MesageResponded(int id);
	void Send(UdpSocket* sock, char* buffer, int length);
	float GetRandomFloat();
};
