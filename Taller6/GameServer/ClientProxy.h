#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include "GeneralInfo.h"
#include <time.h>
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

#define PING_TIME 200
#define RESEND_TIME 500
#define PERCENT_LOSS 25

using namespace sf;
using namespace std;

struct POSITION
{
	uint8_t x, y;
};

struct BufferAndLength {
	char* buffer;
	int length;
};

class ClientProxy {
private:	
	Clock resendClock;
	Clock pingClock;
	
public:	
	map<int, BufferAndLength> msgs2Resend;	
	POSITION position;
	IpAddress ip;	
	unsigned short port;
	string nick;
	int numPing;
	

	ClientProxy(IpAddress myIp, unsigned short myPort, POSITION pos, string n);
	bool operator==(const ClientProxy& other);
	void ResendMsgs(UdpSocket* sock);
	void MesageResponded(int id);
	void Send(UdpSocket* sock, char* buffer, int length);
		
};
