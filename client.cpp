#include "client.h"
#include "globals.h"
#include <stdio.h>

Client::Client() : connected(false)
{
	tcpSocket.setBlocking(false);
	udpSocket.setBlocking(false);

	if (udpSocket.bind(UDP_PORT) != sf::Socket::Done) printf("Could not bind server UDP socket to port %d.\n", UDP_PORT);
}

void Client::update()
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

		packet << "Blargh.";

		if (udpSocket.send(packet, SERVER, UDP_PORT) != sf::Socket::Done) printf("Failed to send data to server.");
	}
}