#include "server.h"
#include <stdio.h>

Server::Server()
{
	if (listener.listen(5060) != sf::Socket::Done) printf("Could not bind TCPListener to port 5060.\n");
	if (socket.bind(5050) != sf::Socket::Done) printf("Could not bind server UDP socket to port 5050.\n");

	listener.setBlocking(false);
	socket.setBlocking(false);
	client.setBlocking(false);
}

void Server::update()
{
	if (listener.accept(client) == sf::Socket::Done)
	{
		printf("Client connected.\n");
	}
}