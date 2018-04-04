#pragma once
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;

struct POSITION
{
	float x, y;
};

class Player
{
private:
	POSITION position;
	sf::CircleShape sprite;
	sf::Color myColor;
	string  myName;
public:
	Player();
	Player(float x, float y, sf::Color myColor);
	~Player();

	bool activated;
	sf::CircleShape Draw(sf::RenderWindow*);
	void setMyPos(float,float);
	POSITION getMyPos();
	sf::Vector2f BoardToWindows(sf::Vector2f);
	bool receivePos();
	void setMyName(string);
	string getMyName();
	
};

