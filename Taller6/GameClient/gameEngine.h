#pragma once

#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"


class gameEngine
{
public:
	Player me;
	vector<Player> others;
	gameEngine();
	~gameEngine();
	void startGame();
	
};

