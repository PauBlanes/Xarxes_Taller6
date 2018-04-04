#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include "gameEngine.h"


using namespace sf;
using namespace std;

gameEngine game;

//Utils : important tenir mateix ordre que els del server
enum RCommands
{
	WC, NEWPLAYER
};
enum SCommands {
	HELLO
};

//Funcions
void ReceiveCommands(UdpSocket* sock);

//Variables per fer conexio
bool welcome;

int main()
{

	IpAddress ip = IpAddress::getLocalAddress();
	Packet helloPacket;
	helloPacket << HELLO;
	UdpSocket socket;
	socket.setBlocking(false);

	//Rutina de conexio
	socket.send(helloPacket, ip, 50000);
	Clock clock;
	clock.restart();
	while (!welcome) {

		//Comprovem si hem rebut el welcome
		ReceiveCommands(&socket);

		//Si han passat 500 ms tornem a enviar missatge hello
		Time currTime = clock.getElapsedTime();		
		if (currTime.asMilliseconds() >  500) {
			socket.send(helloPacket, ip, 50000);
			cout << "sending hello again" << endl;
			clock.restart();
		}
		
	}
	
	game.startGame();

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
			//guardar posicions (la primera és la teva)
			break;
		default:
			break;
		}
	}

}