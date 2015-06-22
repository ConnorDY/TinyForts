#include "server.h"
#include <stdio.h>

Server::Server()
{
	if (socket.bind(54000) != sf::Socket::Done) printf("Could not bind server socket to port 5050.");
}