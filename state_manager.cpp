#include "state_manager.h"

#include "menu_state.h"

StateManager::StateManager(TextureManager const &tM, SoundManager &sM, settings_t &stg)
	: textureManager(tM), soundManager(sM), settings(stg), currentState(new Menu_State(*this, tM, settings))
{
}

StateManager::~StateManager()
{
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