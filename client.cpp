#include "client.h"
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
		sf::Packet packetSend;

		// Send
		if(selfId > 0)
		{
			sf::Time timePassed = sendTimer.getElapsedTime();

			if (timePassed.asMilliseconds() >= 6.0)
			{
				packetSend << sf::Uint8(0) << selfId << p.x << p.y << p.dx << p.dy << p.angle << p.frame << p.scale;
				if (udpSocket.send(packetSend, SERVER, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to server.");
				else sendTimer.restart();
			}
		}
		else printf("No SelfId received yet.");

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

					if (id == 0)
					{
						packetReceive >> p.id;

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
						case 0:
							packetReceive >> p.x >> p.y >> p.dx >> p.dy >> p.angle >> p.frame >> p.scale;

							if (pExists) players[c] = p;
							else players.push_back(p);
							break;

						case 1:
							packetReceive >> selfId;
							printf("Self ID received: %d", selfId);
							break;
					}
				}
			}
		}
	}
}