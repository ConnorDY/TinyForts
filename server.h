#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

class Server
{
	private:
		sf::UdpSocket socket;
		sf::TcpSocket client;
		sf::TcpListener listener;
	public:
		Server();

		void update();
};

#endif