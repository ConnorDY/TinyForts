#include "client.h"
#include "globals.h"
#include <stdio.h>

Client::Client() : connected(false), selfId(-1)
{
	tcpSocket.setBlocking(false);
	udpSocket.setBlocking(false);

	if (udpSocket.bind(UDP_PORT) != sf::Socket::Done) printf("Could not bind server UDP socket to port %d.\n", UDP_PORT);
}

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
			packet << sf::Uint8(0) << p.id << p.x << p.y << p.angle << p.frame << p.scale;

			if (udpSocket.send(packet, SERVER, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to server.");
		}

		// Receive
		sf::IpAddress sender;
		unsigned short port;

		if (udpSocket.receive(packet, sender, port) == sf::Socket::Done)
		{
			#ifdef DEBUG_MODE
			std::cout << "Received " << packet.getDataSize() << " bytes from " << sender << " on port " << port << std::endl;
			#endif

			sf::Uint8 id;
			p.ip = sender;

			if (packet >> id)
			{
				switch (id)
				{
					case 0:
						packet >> p.id >> p.x >> p.y >> p.angle >> p.frame >> p.scale;
						players[p.id] = p;
						break;

					case 1:
						packet >> selfId;
						break;
				}
			}
		}
	}
}