#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "ClientProxy.h"

using namespace sf;
using namespace std;

#define NUM_PLAYERS 2

enum RCommands
{
	HELLO
};
enum SCommands {
	WC
};
void ReceiveCommands();
void AddClientIfNew(ClientProxy newClient);
void SendCommands(ClientProxy client2Send,SCommands cmd2Send);

vector<ClientProxy> clients;
UdpSocket socket;

int main()
{	
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
		ReceiveCommands();		
		
	}

	return 0;
}

void ReceiveCommands() {
	
	Packet rPack;
	IpAddress ipAddr;
	unsigned short newPort;
	if (socket.receive(rPack, ipAddr, newPort) == sf::Socket::Done) {		
		int intCmd;
		rPack >> intCmd;
		RCommands cmd = (RCommands)intCmd;
		
		switch (cmd)
		{		
		case HELLO:
			AddClientIfNew(ClientProxy(ipAddr, newPort));			
			break;
		default:
			break;
		}

	}

	
}

void SendCommands(ClientProxy client2Send, SCommands cmd2Send) {
	
	Packet pack2Send;
	
	switch (cmd2Send) {
	case WC:
		cout << "envio welcome a " << client2Send.ip << ":" << client2Send.port << endl;
		pack2Send << WC;
		socket.send(pack2Send,client2Send.ip, client2Send.port);
		break;
	}
}

void AddClientIfNew(ClientProxy newClient) {
	bool isNew = true;
	//comprovem si és un que ja tenim
	for (int i = 0; i < clients.size(); i++) {
		if (clients[i] == newClient) {
			//estem rebent del mateix client pq ell no ha rebut el welcome
			isNew = false;
		}
	}
	//si es nou l'afegim a l'array
	if (isNew) {
		cout << "new client " << newClient.ip << ":" << newClient.port << endl;
		clients.push_back(newClient); //nomes afegim si és nou
	}
	//en qualsevol cas li enviem el welcome
	SendCommands(newClient, WC);
}


