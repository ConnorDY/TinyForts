#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>
#include "network_player.h"

class Client
{
	private:
		sf::UdpSocket udpSocket;
		sf::TcpSocket tcpSocket;
		std::vector<network_player> players;
		bool connected;
		int selfId;
	public:
		Client();

		void update(network_player p);
};

#endif