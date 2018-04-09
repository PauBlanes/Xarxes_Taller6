#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "ClientProxy.h"
#include "GeneralInfo.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class ServerManager {
	vector<ClientProxy*> clients;
	UdpSocket socket;

	uint8_t posIndex;//temporalment per decidir les posicions

					 //Per controlar acknowledge
	uint8_t packetId;

	Clock pingClock;

public :
	ServerManager();
	void Start();
	void ReceiveCommands();
	void AddClientIfNew(ClientProxy newClient);
	void SendCommands(ClientProxy* client2Send, PacketType cmd2Send);
	ClientProxy* FindClient(IpAddress ip, unsigned short port);
	void PingRoutine();
};