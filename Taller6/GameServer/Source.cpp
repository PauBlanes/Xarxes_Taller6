#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientProxy.h"

using namespace sf;
using namespace std;

#define NUM_PLAYERS 2

enum RCommands
{
	EMPTY, HELLO
};
void ReceiveCommands(UdpSocket* sock);

vector<ClientProxy> clients;

int main()
{
	UdpSocket socket;
	//Connectem el port
	Socket::Status status = socket.bind(50000);
	if (status != sf::Socket::Done)
	{
		cout << "No se puede vincular al puerto" << endl;
	}
	else
		cout << "Port ok" << endl;

	socket.setBlocking(false);	

	//Bucle del joc
	while (true) {
		
		//Comprobamos receive
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
		case EMPTY:
			break;
		case HELLO:

			rPack >> ipAddr >> newPort;
			ClientProxy newClient(ipAddr, newPort);
			clients.push_back(newClient); //nomes afegim si és nou
										  //enviem welcome al client amb totes les posicions
										  //enviem als clients ja conectats la nova pos
			break;
		default:
			break;
		}

	}

	
}