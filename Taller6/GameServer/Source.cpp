#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "ClientProxy.h"
#include "GeneralInfo.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

using namespace sf;
using namespace std;

#define NUM_PLAYERS 4

void ReceiveCommands();
void AddClientIfNew(ClientProxy newClient);
void SendCommands(ClientProxy client2Send,PacketType cmd2Send);
ClientProxy FindClient(IpAddress ip, unsigned short port);
void PingRoutine();

vector<ClientProxy> clients;
UdpSocket socket;

uint8_t posIndex;//temporalment per decidir les posicions

//Per controlar acknowledge
uint8_t packetId; 

Clock pingClock;

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
			cout << cP.numPing << endl;
		}
		//Enviem el ping
		PingRoutine();
		
	}

	return 0;
}

void ReceiveCommands() {	
	
	IpAddress ipAddr;
	unsigned short newPort;

	char rMsg[100];	
	size_t received;

	if (socket.receive(rMsg, 100, received, ipAddr, newPort) == sf::Socket::Done) {
		
		InputMemoryStream ims(rMsg, received);		
		uint8_t cmdInt;		
		ims.Read(&cmdInt);
		PacketType cmd = (PacketType)cmdInt;
				
		switch (cmd)
		{		
		case HELLO:
		{			
			string n = "";		
			//AQUI FALTA REBRE EL STRING
			AddClientIfNew(ClientProxy(ipAddr, newPort, { posIndex, posIndex }, n));
			break;
		}
		case ACK:
			//ens guardem id de missatge
			uint8_t msgId;			
			ims.Read(&msgId);
			//borrem el missatge amb aquest id de la llista de msg que esperen ack
			FindClient(ipAddr, newPort).MesageResponded(msgId);
			
			break;
		default:
			break;
		}
	}	
}

void SendCommands(ClientProxy client2Send, PacketType cmd2Send) {
		
	OutputMemoryStream oms;
	
	switch (cmd2Send) {
	case WC:
	{
		cout << "envio welcome a " << client2Send.ip << ":" << client2Send.port << "|| msgId : " << (int)packetId << endl;
		
		//La capcelera
		oms.Write((uint8_t)PacketType::WC);
		oms.Write(packetId);
		//afegim la nostra pos		
		oms.Write(client2Send.position.x);
		oms.Write(client2Send.position.y);
		posIndex++;//per anar-los posant en diagonal
		//afegim el numero de jugadors
		oms.Write((uint8_t)clients.size());
		//afegim la resta de jugadors
		for (int i = 0; i < clients.size(); i++) {
			oms.Write(clients[i].position.x);
			oms.Write(clients[i].position.y);
		}

		client2Send.Send(&socket, oms.GetBufferPtr(), oms.GetLength());

		//ja que es un missatge critic l'afegim al array dels que esperen resposta
		BufferAndLength temp = { oms.GetBufferPtr(), oms.GetLength() };
		client2Send.msgs2Resend[packetId] = temp;
		packetId++;

		break;
	}
	case NEWPLAYER:
	{
		cout << "envio newPlayer a "  << client2Send.ip << ":" << client2Send.port << "|| msgId : " << (int)packetId << endl;
		
		//La capcelera
		oms.Write((uint8_t)PacketType::NEWPLAYER);		
		oms.Write(packetId);
		//Afegim la pos
		oms.Write(clients[clients.size() - 1].position.x);
		oms.Write(clients[clients.size() - 1].position.y);
		
		client2Send.Send(&socket, oms.GetBufferPtr(), oms.GetLength());

		//ja que es un missatge critic l'afegim al array dels que esperen resposta
		BufferAndLength temp = { oms.GetBufferPtr(), oms.GetLength() };
		client2Send.msgs2Resend[packetId] = temp;
		packetId++;
				
		break;
	}
	case PING:
	{
		//La capcelera
		oms.Write((uint8_t)PacketType::PING);
		oms.Write(packetId);

		client2Send.Send(&socket, oms.GetBufferPtr(), oms.GetLength());

		//sumem al num de ping
		
		
	}
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
		posIndex++;//per anar-los posant en diagonal				  

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

void PingRoutine() {

	//comprovem pings no respostos

	Time currTime = pingClock.getElapsedTime();
	if (currTime.asMilliseconds() >  PING_TIME) {
		OutputMemoryStream oms;
		for each (ClientProxy cP in clients)
		{
			SendCommands(cP, PING);
		}
		
		pingClock.restart();
	}
}









