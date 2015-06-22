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

	if (server != nullptr) server->update();
	if (client != nullptr) client->update();
}

void StateManager::setState(std::unique_ptr<State> state)
{
	currentState = std::move(state);
	stateChanged = true;
}

void StateManager::createServer()
{
	delete client;
	client = nullptr;
	server = new Server();
}

void StateManager::createClient()
{
	delete server;
	server = nullptr;
	client = new Client();
}