#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>
#include "network_player.h"

class Server
{
	private:
		sf::TcpListener listener;
		sf::UdpSocket socket;
		sf::Clock sendTimer;
		std::vector<network_player> clients;
		int clientNum;
	public:
		Server();

		// Accessors
		std::vector<network_player> getOtherPlayers() const;

		// Actions
		void update(network_player p);
};

#endif