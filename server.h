#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

class Server
{
	private:
		sf::TcpListener listener;
		sf::UdpSocket socket;
		std::vector<sf::IpAddress> clients;
		int clientNum;
	public:
		Server();

		void update();
};

#endif