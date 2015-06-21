#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "state_manager.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "input_handler.h"
#include "settings.h"
#include "globals.h"

class State
{
	private:
		StateManager &stateManager;
		sf::View view;
		sf::Clock clock;
	public:
		explicit State(StateManager &sm);
		virtual ~State();

		// Mutators
		void setView(sf::View v);

		// Accessors
		sf::View const& getView() const;
		sf::View& getView();
		sf::Clock const& getClock() const;
		float getViewX() const;
		float getViewY() const;
		StateManager& getStateManager();

		// Actions
		virtual void draw(sf::RenderWindow &window) = 0;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) = 0;
		sf::Time restartClock();
};

#endif