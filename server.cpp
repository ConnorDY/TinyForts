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

		network_player p;

		if (packet >> p.x >> p.y >> p.angle >> p.frame) std::cout << "X: " << p.x << " | Y: " << p.y << " | Angle: " << p.angle << std::endl;
	}
}