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
void Server::update(network_player p)
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

			// Increase client number
			clientNum++;

			// Send them their ID
			sf::Packet packet;
			packet << sf::Uint8(1) << clientNum;
			if (socket.send(packet, ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send client ID.");

			// Create network_player struct
			network_player p;
			p.id = clientNum;
			p.x = -128;
			p.y = -128;
			p.angle = 0;
			p.frame = 0;
			p.scale = 1;
			p.ip = ip;

			// Add to clients vector
			clients.push_back(p);
		}
		else delete client;
	}

	// Current client connections
	sf::Packet packet;

	// Send
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		network_player p1 = clients.at(i);

		// Send host to all clients
		packet << sf::Uint8(0) << p.id << p.x << p.y << p.angle << p.frame << p.scale;
		if (socket.send(packet, p1.ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to client %d.", i);

		for (unsigned int j = 0; j < clients.size(); j++)
		{
			// Send all clients to all clients
			network_player p2 = clients.at(j);

			packet << sf::Uint8(0) << p2.id << p2.x << p2.y << p2.angle << p2.frame << p2.scale;
			if (socket.send(packet, p1.ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to client %d.", i);
		}
	}

	// Receive
	sf::IpAddress sender;
	unsigned short port;

	for (int k = 0; k < 10; k++)
	{
		if (socket.receive(packet, sender, port) == sf::Socket::Done)
		{
			#ifdef DEBUG_MODE
			std::cout << "Received " << packet.getDataSize() << " bytes from " << sender << " on port " << port << std::endl;
			#endif

			sf::Uint8 id;
			int c = -1;
			p.ip = sender;

			if (packet >> id >> c)
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
}