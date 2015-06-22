#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

class Server
{
	private:
		sf::UdpSocket socket;
	public:
		Server();
};

#endif