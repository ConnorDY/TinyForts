#include "server.h"
#include "bullet.h"
#include "room.h"
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
void Server::sendBullet(network_bullet b)
{
	// Send bullet to all other clients
	sf::Packet packetSend;
	packetSend << sf::Uint8(2) << b.x << b.y << b.angle << b.speed;

	for (unsigned int i = 0; i < clients.size(); i++)
	{
		if (socket.send(packetSend, clients.at(i).ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to client %d.", i);
	}
}

void Server::update(Room &room, network_player playerHost)
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

	// Send
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		sf::Time timePassed = sendTimer.getElapsedTime();
			
		if (timePassed.asMilliseconds() >= TICK_TIME)
		{
			sf::Packet packetSend;

			network_player playerClientTo = clients.at(i);

			// Send host to all clients
			packetSend << sf::Uint8(0) << 0 << playerHost.x << playerHost.y << playerHost.dx << playerHost.dy << playerHost.angle << playerHost.frame << playerHost.scale;
			if (socket.send(packetSend, playerClientTo.ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to client %d.", i);

			for (unsigned int j = 0; j < clients.size(); j++)
			{
				// Send all clients to all clients
				network_player playerClient = clients.at(j);

				packetSend << sf::Uint8(0) << playerClient.id << playerClient.x << playerClient.y << playerClient.dx << playerClient.dy << playerClient.angle << playerClient.frame << playerClient.scale;
				if (socket.send(packetSend, playerClientTo.ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to client %d.", i);
			}

			sendTimer.restart();
		}
	}

	// Receive
	for (int k = 0; k < 10; k++)
	{
		sf::Packet packetReceive;
		network_player playerReceive;
		sf::IpAddress sender;
		unsigned short port;

		if (socket.receive(packetReceive, sender, port) == sf::Socket::Done)
		{
			#ifdef DEBUG_MODE
			std::cout << "Received " << packetReceive.getDataSize() << " bytes from " << sender << " on port " << port << std::endl;
			#endif

			sf::Uint8 id;
			int cId = -1;
			playerReceive.ip = sender;

			if (packetReceive >> id)
			{
				switch (id)
				{
					// Player Position
					case 0:
						packetReceive >> cId >> playerReceive.x >> playerReceive.y >> playerReceive.dx >> playerReceive.dy >> playerReceive.angle >> playerReceive.frame >> playerReceive.scale;
						clients[cId - 1] = playerReceive;
						break;

					// Disconnect
					case 1:
						
						break;

					// Bullet fired
					case 2:
						network_bullet b;
						packetReceive >> b.x >> b.y >> b.angle >> b.speed;
						room.spawn(new Bullet(room, b.x, b.y, b.speed, b.angle));
						sendBullet(b);
						break;
				}
			}
		}
	}
}