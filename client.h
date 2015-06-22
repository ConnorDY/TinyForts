#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>

class Client
{
	private:
		sf::UdpSocket udpSocket;
		sf::TcpSocket tcpSocket;
		bool connected;
	public:
		Client();

		void update(double x, double y, double angle);
};

#endif