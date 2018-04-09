#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include "Player.h"
#include "GeneralInfo.h"
#include "OutputMemoryStream.h"

using namespace sf;
using namespace std;

class gameEngine
{
private:
	bool welcome;
	UdpSocket socket;
	string nick;
public:
	Player me;
	vector<Player> others;
	gameEngine();
	~gameEngine();
	void startGame();	
	void ReceiveCommands();
	void SendCommands(PacketType cmd); //FER RANDOM PER NOMES ENVIAR A VEGADES
	void SendACK(int msgId);
};

