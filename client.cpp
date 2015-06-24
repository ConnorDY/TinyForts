#include "client.h"
#include "bullet.h"
#include "room.h"
#include "globals.h"
#include <stdio.h>
#include <iostream>

Client::Client() : connected(false), selfId(-3)
{
	tcpSocket.setBlocking(false);
	udpSocket.setBlocking(false);

	if (udpSocket.bind(UDP_PORT) != sf::Socket::Done) printf("Could not bind server UDP socket to port %d.\n", UDP_PORT);
}


/* Accessors */
std::vector<network_player> Client::getOtherPlayers() const
{
	return players;
}

int Client::getSelfId() const
{
	return selfId;
}


/* Actions */
void Client::sendToServer(sf::Packet packetSend)
{
	if (udpSocket.send(packetSend, SERVER, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to server.");
}

void Client::sendBullet(network_bullet b)
{
	// Send bullet to server
	sf::Packet packetSend;
	packetSend << sf::Uint8(2) << selfId << b._id.id << b.x << b.y << b.angle << b.speed;
	sendToServer(packetSend);
}

void Client::sendDelete(object_id id_d)
{
	// Send delete to server
	sf::Packet packetSend;
	packetSend << sf::Uint8(3) << selfId << id_d.id;
	sendToServer(packetSend);
}


void Client::update(Room &room, network_player p)
{
	if (!connected)
	{
		sf::Socket::Status status = tcpSocket.connect(SERVER, TCP_PORT);

		if (status != sf::Socket::Done) printf("Failed to connect to server. Retrying...\n");
		else
		{
			printf("Connected to server!\n");
			connected = true;
		}
	}
	else
	{
		sf::Packet packetSend;

		// Send
		if(selfId > 0)
		{
			sf::Time timePassed = sendTimer.getElapsedTime();

			if (timePassed.asMilliseconds() >= TICK_TIME)
			{
				packetSend << sf::Uint8(0) << selfId << p.x << p.y << p.dx << p.dy << p.angle << p.frame << p.scale;
				sendToServer(packetSend);
				sendTimer.restart();
			}
		}
		else printf("No SelfId received yet.\n");

		// Receive
		sf::IpAddress sender;
		unsigned short port;

		for (unsigned int k = 0; k < 10; k++)
		{
			sf::Packet packetReceive;

			if (udpSocket.receive(packetReceive, sender, port) == sf::Socket::Done)
			{
				#ifdef DEBUG_MODE
				std::cout << "Received " << packetReceive.getDataSize() << " bytes from " << sender << " on port " << port << std::endl;
				#endif

				sf::Uint8 id;
				p.ip = sender;

				if (packetReceive >> id)
				{
					bool pExists = false;
					int c = -1;

					if (id == 0 || id == 2 || id == 3)
					{
						packetReceive >> p.id;
						if (p.id == selfId) return;

						for (unsigned int i = 0; i < players.size(); i++)
						{
							if (players.at(i).id == p.id)
							{
								pExists = true;
								c = i;
								break;
							}
						}
					}

					switch (id)
					{
						// Player position
						case 0:
							packetReceive >> p.x >> p.y >> p.dx >> p.dy >> p.angle >> p.frame >> p.scale;
							if (pExists) players[c] = p;
							else players.push_back(p);
							break;

						// Client ID
						case 1:
							packetReceive >> selfId;
							printf("Self ID received: %d\n", selfId);
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
							break;

						// Delete object
						case 3:
							printf("Received object deletion request.\n");

							object_id id_d;
							packetReceive >> id_d.owner >> id_d.id;
							room.deleteObj(id_d);
							break;
					}
				}
			}
		}
	}
}