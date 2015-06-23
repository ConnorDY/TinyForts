#include "client.h"
#include "globals.h"
#include <stdio.h>

Client::Client() : connected(false)
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

		packet << sf::Uint8(0) << p.x << p.y << p.angle << p.frame << p.scale;

		if (udpSocket.send(packet, SERVER, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to server.");
	}
}