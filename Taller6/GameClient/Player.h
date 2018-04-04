#pragma once
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>

struct POSITION
{
	float x, y;
};

class Player
{
private:
	POSITION ActualPos;
	sf::CircleShape MyPos;
	bool existPos=false;

public:
	Player();
	~Player();

	sf::CircleShape ShowMyPosition(POSITION);
	void setMyPos(float,float);
	POSITION getMyPos();
	sf::Vector2f BoardToWindows(sf::Vector2f);
	bool receivePos();
};

