#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include "Player.h"

using namespace sf;
using namespace std;

//Utils : important tenir mateix ordre que els del server
enum RCommands
{
	WC, NEWPLAYER
};
enum SCommands {
	HELLO
};

class gameEngine
{
private:
	bool welcome;
	UdpSocket socket;
public:
	Player me;
	vector<Player> others;
	gameEngine();
	~gameEngine();
	void startGame();	
	void ReceiveCommands();
};

