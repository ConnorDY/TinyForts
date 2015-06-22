#include "state_manager.h"

#include "menu_state.h"

StateManager::StateManager(TextureManager const &tM, SoundManager &sM, settings_t &stg)
	: textureManager(tM), soundManager(sM), settings(stg), currentState(new Menu_State(*this, tM, settings)),
	server(nullptr), client(nullptr)
{
}

StateManager::~StateManager()
{
}


/* Accessors */
Server* StateManager::getServer()
{
	return server;
}

Client* StateManager::getClient()
{
	return client;
}



/* Actions */
void StateManager::draw(sf::RenderWindow &window)
{
	currentState->draw(window);
}

void StateManager::update(sf::RenderWindow &window)
{
	do {
		stateChanged = false;
		currentState->update(window, soundManager, inputHandler);
	} while (stateChanged);
}

void StateManager::setState(std::unique_ptr<State> state)
{
	currentState = std::move(state);
	stateChanged = true;
}

void StateManager::resetNetworking()
{
	delete server;
	delete client;
	server = nullptr;
	client = nullptr;
}

void StateManager::createServer()
{
	resetNetworking();
	server = new Server();
}

void StateManager::createClient()
{
	resetNetworking();
	client = new Client();
}