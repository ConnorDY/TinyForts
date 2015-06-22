#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

class Client
{
	private:
		sf::UdpSocket udpSocket;
		sf::TcpSocket tcpSocket;
	public:
		Client();

		void update();
};

#endif