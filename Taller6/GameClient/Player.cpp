#include "Player.h"

#define MAX 100
#define SIZE_TABLERO 64
#define SIZE_FILA_TABLERO 8
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define OFFSET_AVATAR 5

#define SIZE_TABLERO 64
#define LADO_CASILLA 64
#define RADIO_AVATAR 25.f
#define OFFSET_AVATAR 5



Player::Player()
{
}


Player::~Player()
{

}

sf::CircleShape Player::ShowMyPosition(POSITION p) {
	MyPos.setRadius(RADIO_AVATAR);
	MyPos.setFillColor(sf::Color::Yellow);
	sf::Vector2f M_posicion(p.x,p.y);
	M_posicion = BoardToWindows(M_posicion);
	MyPos.setPosition(M_posicion);

	return MyPos;
}

void Player::setMyPos(float x,float y) {
	ActualPos = {x,y};
	
	existPos = true;
};
POSITION Player::getMyPos() {
	return ActualPos;
};

sf::Vector2f Player::BoardToWindows(sf::Vector2f _position)
{
	return sf::Vector2f(_position.x*LADO_CASILLA + OFFSET_AVATAR, _position.y*LADO_CASILLA + OFFSET_AVATAR);
}

bool Player::receivePos() {
	return existPos;
}

void Player::setMyName(string name) {
	myName = name;
}

string Player::getMyName() {
	return myName;
}
