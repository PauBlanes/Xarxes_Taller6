#pragma once

#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"


class gameEngine
{
public:
	gameEngine();
	~gameEngine();
	void startGame();
};

