#include "server.h"
#include "globals.h"
#include <stdio.h>
#include <iostream>

Server::Server() : clientNum(0)
{
	listener.setBlocking(false);

	clients.reserve(3);

	if (listener.listen(TCP_PORT) != sf::Socket::Done) printf("Could not bind TCP Listener to port %d.\n", TCP_PORT);
	if (socket.bind(UDP_PORT) != sf::Socket::Done) printf("Could not bind UDP Socket to port %d.\n", UDP_PORT);
}

void Server::update()
{
	// New client connections
	if (clientNum < 3)
	{
		sf::TcpSocket* client = new sf::TcpSocket;
		client->setBlocking(false);
		
		if (listener.accept(*client) == sf::Socket::Done)
		{
			sf::IpAddress ip = client->getRemoteAddress();
			std::cout << "Client connected: " << ip.toString() << std::endl;

			clients.push_back(ip);
			clientNum++;
		}
		else delete client;
	}

	// Current client connections
	for (std::vector<sf::IpAddress>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		sf::IpAddress client = *it;

		std::size_t received;
		unsigned short port;
		char data[100];

		if (socket.receive(data, 100, received, client, port) == sf::Socket::Done)
		{
			printf("Received data: %s!\n", data);
		}
	}
}