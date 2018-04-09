#include "ServerManager.h"

ServerManager::ServerManager() {

}
void ServerManager::Start() {
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
		for (int i = 0; i < clients.size(); i++)
		{
			clients[i]->ResendMsgs(&socket);
			
		}
		//Enviem el ping
		PingRoutine();

	}
}
void ServerManager::ReceiveCommands() {

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
			cout << (int)msgId << endl;
			FindClient(ipAddr, newPort)->MesageResponded((int)msgId);

			break;
		default:
			break;
		}
	}
}

void ServerManager::SendCommands(ClientProxy* client2Send, PacketType cmd2Send) {

	OutputMemoryStream oms;

	switch (cmd2Send) {
	case WC:
	{
		cout << "envio welcome a " << client2Send->ip << ":" << client2Send->port << "|| msgId : " << (int)packetId << endl;

		//La capcelera
		oms.Write((uint8_t)PacketType::WC);
		oms.Write(packetId);
		//afegim la nostra pos		
		oms.Write(client2Send->position.x);
		oms.Write(client2Send->position.y);

		//afegim el numero de jugadors
		//cout << clients.size() << endl;
		oms.Write((uint8_t)clients.size());
		//afegim la resta de jugadors
		for (int i = 0; i < clients.size(); i++) {
			oms.Write(clients[i]->position.x);
			oms.Write(clients[i]->position.y);
		}

		client2Send->Send(&socket, oms.GetBufferPtr(), oms.GetLength());

		//ja que es un missatge critic l'afegim al array dels que esperen resposta
		BufferAndLength temp = { oms.GetBufferPtr(), oms.GetLength() };
		client2Send->msgs2Resend[(int)packetId] = temp;		
		packetId++;

		break;
	}
	case NEWPLAYER:
	{
		cout << "envio newPlayer a " << client2Send->ip << ":" << client2Send->port << "|| msgId : " << (int)packetId << endl;

		//La capcelera
		oms.Write((uint8_t)PacketType::NEWPLAYER);
		oms.Write(packetId);
		//Afegim la pos
		oms.Write(clients[clients.size() - 1]->position.x);
		oms.Write(clients[clients.size() - 1]->position.y);

		client2Send->Send(&socket, oms.GetBufferPtr(), oms.GetLength());

		//ja que es un missatge critic l'afegim al array dels que esperen resposta
		BufferAndLength temp = { oms.GetBufferPtr(), oms.GetLength() };
		client2Send->msgs2Resend[(int)packetId] = temp;
		packetId++;

		break;
	}
	case PING:
	{
		//La capcelera
		oms.Write((uint8_t)PacketType::PING);
		oms.Write(packetId);

		client2Send->Send(&socket, oms.GetBufferPtr(), oms.GetLength());

		//sumem al num de ping
		client2Send->numPing += 1;

	}
	default:
		break;
	}
}

void ServerManager::AddClientIfNew(ClientProxy newClient) {
	bool isNew = true;
	//comprovem si és un que ja tenim
	for (int i = 0; i < clients.size(); i++) {
		if (*clients[i] == newClient) {
			//estem rebent del mateix client pq ell no ha rebut el welcome
			isNew = false;
			SendCommands(clients[i], WC);
		}
	}	

	//si es nou l'afegim a l'array i enviem pos als altres
	if (isNew) {
		posIndex++;//per anar-los posant en diagonal

		cout << " rebut new client " << newClient.ip << ":" << newClient.port << endl;
		clients.push_back(&newClient); //nomes afegim si és nou

		SendCommands(&newClient, WC);

		for (int i = 0; i < clients.size() - 1; i++) { //Per enviar a tots menys lultim i no ho posem abans del pushback per poder agafar la pos daquest client des del array de clients
			SendCommands(clients[i], NEWPLAYER);
		}
	}

}

ClientProxy* ServerManager::FindClient(IpAddress ip, unsigned short port) {
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i]->ip == ip && clients[i]->port == port)
			return clients[i];
	}
}

void ServerManager::PingRoutine() {

	//comprovem pings no respostos

	Time currTime = pingClock.getElapsedTime();
	if (currTime.asMilliseconds() >  PING_TIME) {
		OutputMemoryStream oms;
		for (int i = 0; i < clients.size(); i++)
		{
			SendCommands(clients[i], PING);
		}

		pingClock.restart();
	}
}
