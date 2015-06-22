#include "client.h"
#include <stdio.h>

Client::Client()
{
	tcpSocket.setBlocking(false);
	udpSocket.setBlocking(false);

	if (udpSocket.bind(5050) != sf::Socket::Done) printf("Could not bind server UDP socket to port 5050.\n");
}

void Client::update()
{
	sf::Socket::Status status = tcpSocket.connect("192.168.1.87", 5060);

	if (status != sf::Socket::Done) printf("Failed to connect to server. Retrying...\n");
	else printf("Connected to server!\n");
}