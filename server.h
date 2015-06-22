#ifndef SERVER_H
#define SERVER_H

#include <SFML/Network.hpp>

class Server
{
	private:
		sf::UdpSocket socket;
		sf::TcpListener listener;
		std::vector<sf::TcpSocket*> clients;
		sf::SocketSelector selector;
		int clientNum;
	public:
		Server();

		void update();
};

#endif