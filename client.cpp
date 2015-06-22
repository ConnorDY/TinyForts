#include "client.h"
#include <stdio.h>

Client::Client()
{
	if (udpSocket.bind(5050) != sf::Socket::Done) printf("Could not bind server UDP socket to port 5060.\n");

	status = tcpSocket.connect("192.168.1.87", 5060);
	tcpSocket.setBlocking(false);
	udpSocket.setBlocking(false);
}

void Client::update()
{
	if (status != sf::Socket::Done) printf("Could not connect to server.\n");
}