#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>
#include "network_player.h"

class Client
{
	private:
		sf::UdpSocket udpSocket;
		sf::TcpSocket tcpSocket;
		bool connected;
	public:
		Client();

		void update(network_player p);
};

#endif