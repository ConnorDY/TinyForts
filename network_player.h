#ifndef NETWORK_PLAYER_H
#define NETWORK_PLAYER_H

#include <SFML/Network.hpp>

struct network_player
{
	sf::IpAddress ip;
	double x, y, angle;
	int id, frame, scale;
};

#endif