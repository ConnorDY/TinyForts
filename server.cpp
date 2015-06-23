#include "server.h"
#include "globals.h"
#include <stdio.h>
#include <iostream>

Server::Server() : clientNum(0)
{
	listener.setBlocking(false);
	socket.setBlocking(false);

	clients.reserve(3);

	if (listener.listen(TCP_PORT) != sf::Socket::Done) printf("Could not bind TCP Listener to port %d.\n", TCP_PORT);
	if (socket.bind(UDP_PORT) != sf::Socket::Done) printf("Could not bind UDP Socket to port %d.\n", UDP_PORT);
}


/* Accessors */
std::vector<network_player> Server::getOtherPlayers() const
{
	return clients;
}


/* Actions */
void Server::update()
{
	// New client connections
	if (clientNum < 3)
	{
		sf::TcpSocket* client = new sf::TcpSocket;
		client->setBlocking(false);
		
		if (listener.accept(*client) == sf::Socket::Done)
		{
			sf::IpAddress ip = client->getRemoteAddress();
			std::cout << "Client connected: " << ip.toString() << std::endl;

			network_player p;
			p.x = -128;
			p.y = -128;
			p.angle = 0;
			p.frame = 0;
			p.scale = 1;
			p.ip = ip;

			clients.push_back(p);
			clientNum++;
		}
		else delete client;
	}

	// Current client connections
	sf::Packet packet;
	sf::IpAddress sender;
	unsigned short port;

	if (socket.receive(packet, sender, port) == sf::Socket::Done)
	{
		std::cout << "Received " << packet.getDataSize() << " bytes from " << sender << " on port " << port << std::endl;

		sf::Uint8 id;
		network_player p;
		int c = -1;
		p.ip = sender;

		for (unsigned int i = 0; i < clients.size(); i++)
		{
			if (clients.at(i).ip == sender)
			{
				c = i;
				break;
			}
		}

		if (packet >> id)
		{
			switch (id)
			{
				case 0:
					packet >> p.x >> p.y >> p.angle >> p.frame >> p.scale;
					clients[c] = p;
					break;
			}
		}
	}
}