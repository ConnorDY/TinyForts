#include "client.h"
#include "globals.h"
#include <stdio.h>
#include <iostream>

Client::Client() : connected(false), selfId(-1)
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
void Client::update(network_player p)
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
		sf::Packet packet;

		// Send
		if(selfId > 0)
		{
			packet << sf::Uint8(0) << p.id << p.x << p.y << p.dx << p.dy << p.angle << p.frame << p.scale;
			if (udpSocket.send(packet, SERVER, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to server.");
		}

		// Receive
		sf::IpAddress sender;
		unsigned short port;

		for (unsigned int k = 0; k < 10; k++)
		{
			if (udpSocket.receive(packet, sender, port) == sf::Socket::Done)
			{
				#ifdef DEBUG_MODE
				std::cout << "Received " << packet.getDataSize() << " bytes from " << sender << " on port " << port << std::endl;
				#endif

				sf::Uint8 id;
				p.ip = sender;

				if (packet >> id)
				{
					bool pExists = true;
					int c = -1;

					if (id == 0)
					{
						packet >> p.id;

						for (unsigned int i = 0; i < players.size(); i++)
						{
							if (players.at(i).id == p.id)
							{
								c = i;
								break;
							}
						}

						if (c < 0) pExists = false;
					}

					switch (id)
					{
						case 0:
							packet >> p.x >> p.y >> p.dx >> p.dy >> p.angle >> p.frame >> p.scale;

							if (pExists) players[c] = p;
							else players.push_back(p);
							break;

						case 1:
							packet >> selfId;
							break;
					}
				}
			}
		}
	}
}