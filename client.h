#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

#include "network_player.h"
#include "network_bullet.h"

class Room;

class Client
{
	private:
		sf::UdpSocket udpSocket;
		sf::TcpSocket tcpSocket;
		sf::Clock sendTimer;
		std::vector<network_player> players;
		bool connected;
		int selfId;
	public:
		Client();

		// Accessors
		std::vector<network_player> getOtherPlayers() const;
		int getSelfId() const;

		// Actions
		void sendToServer(sf::Packet packetSend);
		void sendBullet(network_bullet b);
		void sendDelete(unsigned int n);
		void update(Room &room, network_player p);
};

#endif