#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

#include "network_player.h"
#include "network_bullet.h"
#include "object_id.h"

class Room;

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
		void sendToAll(sf::Packet packetSend);
		void sendToAll(sf::Packet packetSend, unsigned int e);
		void sendBullet(network_bullet b);
		void sendDelete(object_id id_d);
		void update(Room &room, network_player p);
};

#endif