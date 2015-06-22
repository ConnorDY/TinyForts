#include "server.h"
#include "globals.h"
#include <stdio.h>

Server::Server()
{
	listener.setBlocking(false);
	socket.setBlocking(false);
	client.setBlocking(false);

	if (listener.listen(TCP_PORT) != sf::Socket::Done) printf("Could not bind TCPListener to port %d.\n", TCP_PORT);
	if (socket.bind(UDP_PORT) != sf::Socket::Done) printf("Could not bind server UDP socket to port %d.\n", UDP_PORT);
}

void Server::update()
{
	if (listener.accept(client) == sf::Socket::Done)
	{
		printf("Client connected.\n");
	}
}