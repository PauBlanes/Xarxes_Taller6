#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>

using namespace sf;
using namespace std;

enum RCommands
{
	WC
};
enum SCommands {
	HELLO
};

bool welcome;

void ReceiveCommands(UdpSocket* sock);

int main()
{
	
	
	IpAddress ip = IpAddress::getLocalAddress();
	Packet helloPacket;
	helloPacket << HELLO;
	UdpSocket socket;

	//Enviem hello fins a rebre el ack
	socket.send(helloPacket, ip, 50000);
	while (!welcome) {		
		ReceiveCommands(&socket);
	}
	

	return 0;
}

void ReceiveCommands(UdpSocket* sock) {
	Packet rPack;
	IpAddress ipAddr;
	unsigned short newPort;

	if (sock->receive(rPack, ipAddr, newPort) == sf::Socket::Done) {
		int intCmd;
		rPack >> intCmd;
		RCommands cmd = (RCommands)intCmd;
		
		switch (cmd)
		{		
		case WC:
			cout << "benvingut" << endl;
			welcome = true;
			break;
		default:
			break;
		}
	}

}