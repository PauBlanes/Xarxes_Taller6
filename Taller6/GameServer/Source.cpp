#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "ClientProxy.h"
#include "GeneralInfo.h"
#include "InputMemoryStream.h"

using namespace sf;
using namespace std;

#define NUM_PLAYERS 4

void ReceiveCommands();
void AddClientIfNew(ClientProxy newClient);
void SendCommands(ClientProxy client2Send,PacketType cmd2Send);
ClientProxy FindClient(IpAddress ip, unsigned short port);

vector<ClientProxy> clients;
UdpSocket socket;

int posIndex;//temporalment per decidir les posicions

//Per controlar acknowledge
int packetId;

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

		//La propia funcio dels clients ja controla que ha d'enviar cada 200 ms
		for each (ClientProxy cP in clients)
		{
			cP.ResendMsgs(&socket);
		}	
		
	}

	return 0;
}

void ReceiveCommands() {
	
	Packet rPack;
	IpAddress ipAddr;
	unsigned short newPort;

	char rMsg[100];	
	size_t received;

	//if (socket.receive(rPack, ipAddr, newPort) == sf::Socket::Done) {
	if (socket.receive(rMsg, 100, received, ipAddr, newPort) == sf::Socket::Done) {
		
		InputMemoryStream ims(rMsg, received);
		PacketType cmd;
		ims.Read(&cmd);
		
		//int intCmd;
		//rPack >> intCmd;
		//PacketType cmd = (PacketType)intCmd;
		
		switch (cmd)
		{		
		case HELLO:
		{
			cout << "es bien" << endl;
			/*string n;
			rPack >> n;
			AddClientIfNew(ClientProxy(ipAddr, newPort, { posIndex, posIndex }, n));*/
			break;
		}
		case ACK:
			//ens guardem id de missatge
			int msgId;
			rPack >> msgId;
			
			//borrem el missatge amb aquest id de la llista de msg que esperen ack
			FindClient(ipAddr, newPort).MesageResponded(msgId);
			
			break;
		default:
			break;
		}

	}

	
}

void SendCommands(ClientProxy client2Send, PacketType cmd2Send) {
	
	//FER RANDOM PER NOMES ENVIAR A VEGADES
	Packet pack2Send;
	
	switch (cmd2Send) {
	case WC:
		cout << "envio welcome a " << client2Send.nick << client2Send.ip << ":" << client2Send.port << endl;
		pack2Send << WC;
		
		//afegim la nostra pos
		int newX, newY;
		pack2Send << client2Send.position.x << client2Send.position.y;
		posIndex++;
		
		//afegim el numero de jugadors		
		pack2Send << (int)clients.size();

		//afegim la resta de jugadors
		for (int i = 0; i < clients.size(); i++) {					
			pack2Send << clients[i].position.x << clients[i].position.y;			
		}

		socket.send(pack2Send,client2Send.ip, client2Send.port);
		break;
	case NEWPLAYER:
		cout << "envio newPlayer a " << client2Send.nick << client2Send.ip << ":" << client2Send.port << endl;
		pack2Send << NEWPLAYER;
		pack2Send << packetId; packetId++; //per saber despres el acknowledge a quin misatge respon
		pack2Send << clients[clients.size()-1].position.x << clients[clients.size() - 1].position.y;
		socket.send(pack2Send, client2Send.ip, client2Send.port);
		break;
	default:
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
	
	//tant si es nou com si no li enviem el welcome	
	SendCommands(newClient, WC);	
	
	//si es nou l'afegim a l'array i enviem pos als altres
	if (isNew) {
				
		cout << "new client " << newClient.ip << ":" << newClient.port << endl;
		clients.push_back(newClient); //nomes afegim si és nou

		for (int i = 0; i < clients.size() - 1; i++) { //Per enviar a tots menys lultim i no ho posem abans del pushback per poder agafar la pos daquest client des del array de clients
			SendCommands(clients[i], NEWPLAYER);
		}
	}	
	
}

ClientProxy FindClient(IpAddress ip, unsigned short port) {
	for each (ClientProxy cP in clients)
	{
		if (cP.ip == ip && cP.port == port)
			return cP;
	}
}






