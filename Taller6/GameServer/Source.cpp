#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "ClientProxy.h"

using namespace sf;
using namespace std;

#define NUM_PLAYERS 2

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

	//Esperem als clients i ens els guardem a mida que arriven
	vector<ClientProxy> clients;
	while (clients.size() < NUM_PLAYERS) {
		
		//Esperamos a recibir algo
		char data[100];
		sf::IpAddress ipAddr;
		unsigned short newPort;
		std::size_t received;
		if (socket.receive(data, 100, received, ipAddr, newPort) != sf::Socket::Done)
			cout << "Error al recibir" << endl;
		else {
			ClientProxy newClient(ipAddr, newPort);
			clients.push_back(newClient);
		}
		
	}
	
	return 0;
}