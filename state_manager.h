#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

#include "settings.h"
#include "input_handler.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "server.h"
#include "client.h"

class State;

class StateManager
{
	private:
		TextureManager const &textureManager;
		SoundManager &soundManager;
		InputHandler inputHandler;
		Server *server;
		Client *client;
		settings_t &settings;
		std::unique_ptr<State> currentState;
		bool stateChanged;

		void resetNetworking();
	public:
		StateManager(TextureManager const &tM, SoundManager &sM, settings_t &settings);
		~StateManager();

		// Accessors
		Server* getServer();
		Client* getClient();

		// Actions
		void draw(sf::RenderWindow &window);
		void update(sf::RenderWindow &window);
		void setState(std::unique_ptr<State> state);
		void createServer();
		void createClient();
};

#endif