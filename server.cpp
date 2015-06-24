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
void Server::sendToAll(sf::Packet packetSend)
{
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		if (socket.send(packetSend, clients.at(i).ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to client %d.", i);
	}
}

void Server::sendToAll(sf::Packet packetSend, unsigned int e)
{
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		if (i == e - 1) continue;
		else if (socket.send(packetSend, clients.at(i).ip, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to client %d.", i);
	}
}

void Server::sendBullet(network_bullet b)
{
	// Send bullet to all other clients
	sf::Packet packetSend;
	packetSend << sf::Uint8(2) << b._id.owner << b._id.id << b.x << b.y << b.angle << b.speed;
	sendToAll(packetSend, b._id.owner);
}

void Server::sendDelete(object_id id_d)
{
	// Send delete object request to all other clients
	sf::Packet packetSend;
	packetSend << sf::Uint8(3) << id_d.owner << id_d.id;
	sendToAll(packetSend);
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
	sf::Time timePassed = sendTimer.getElapsedTime();

	if (timePassed.asMilliseconds() >= TICK_TIME)
	{
		// Send host player to all clients
		sf::Packet packetHost;
		packetHost << sf::Uint8(0) << 0 << playerHost.x << playerHost.y << playerHost.dx << playerHost.dy << playerHost.angle << playerHost.frame << playerHost.scale;
		sendToAll(packetHost);

		for (unsigned int i = 0; i < clients.size(); i++)
		{
			sf::Packet packetSend;
			network_player playerClient = clients.at(i);
			packetSend << sf::Uint8(0) << playerClient.id << playerClient.x << playerClient.y << playerClient.dx << playerClient.dy << playerClient.angle << playerClient.frame << playerClient.scale;
			sendToAll(packetSend, i);
		}

		sendTimer.restart();
	}

	// Receive
	for (int k = 0; k < 30; k++)
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
						printf("Received bullet creation request.\n");
						network_bullet b;
						packetReceive >> b._id.owner >> b._id.id >> b.x >> b.y >> b.angle >> b.speed;

						Bullet *bul;
						bul = new Bullet(room, b.x, b.y, b.speed, b.angle);
						bul->setId(b._id);

						room.spawn(bul);
						sendBullet(b);
						break;

					// Delete object
					case 3:
						printf("Received object deletion request.\n");
						object_id id_d;
						packetReceive >> id_d.owner >> id_d.id;
						room.deleteObj(id_d);
						sendDelete(id_d);
						break;
				}
			}
		}
	}
}