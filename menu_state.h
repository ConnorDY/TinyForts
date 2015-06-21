#ifndef MENU_STATE_H
#define MENU_STATE_H

#include <vector>

#include "globals.h"
#include "state.h"

class Menu_State : public State
{
	private:
		TextureManager const &textureManager;
		settings_t &settings;
		sf::Font fnt;
		unsigned int currentOption = 0;
		std::vector<std::string> menuOptions;
	public:
		Menu_State(StateManager &sM, TextureManager const &textureManager, settings_t &settings);
		virtual ~Menu_State();

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif