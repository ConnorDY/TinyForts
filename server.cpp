#include "server.h"
#include "globals.h"
#include <stdio.h>

Server::Server() : clientNum(0)
{
	listener.setBlocking(false);
	socket.setBlocking(false);

	clients.reserve(3);
	selector.add(listener);

	if (listener.listen(TCP_PORT) != sf::Socket::Done) printf("Could not bind TCPListener to port %d.\n", TCP_PORT);
	if (socket.bind(UDP_PORT) != sf::Socket::Done) printf("Could not bind server UDP socket to port %d.\n", UDP_PORT);
}

void Server::update()
{
	// The listener is ready: there is a pending connection
	if (clientNum < 3 && selector.isReady(listener))
	{	
		sf::TcpSocket* client = new sf::TcpSocket;
		client->setBlocking(false);

		if (listener.accept(*client) == sf::Socket::Done)
		{
			printf("Client connected.\n");

			clients.push_back(client);
			selector.add(*client);
			
			clientNum++;
		}
		else delete client;
	}
	else
	{
		// The listener socket is not ready, test all other sockets (the clients)
		for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			sf::TcpSocket& client = **it;
			if (selector.isReady(client))
			{
				// The client has sent some data, we can receive it
				sf::Packet packet;
				if (client.receive(packet) == sf::Socket::Done)
				{
					
				}
			}
		}
	}
}