#include "state.h"

State::State(StateManager &sm) : stateManager(sm)
{

}

State::~State()
{

}


// Mutators
void State::setView(sf::View v) { view = v; }


// Accesors
sf::View const& State::getView() const { return view; }
sf::View& State::getView() { return view; }
sf::Clock const& State::getClock() const { return clock; }

float State::getViewX() const
{
	return view.getCenter().x - (VIEW_WIDTH / 2.0f);
}

float State::getViewY() const
{
	return view.getCenter().y - (VIEW_HEIGHT / 2.0f);
}

StateManager& State::getStateManager()
{
	return stateManager;
}


// Actions
sf::Time State::restartClock()
{
	return clock.restart();
}