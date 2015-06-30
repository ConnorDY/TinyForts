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
		sf::Color bg;
		unsigned int currentOption = 0;
		std::vector<std::string> menuOptions;

		void drawText(sf::RenderWindow &window, double x, double y, unsigned int size, sf::Color colour, unsigned int alignh, unsigned int alignv, std::string str);
		void drawTab(sf::RenderWindow &window, double x, double y, double width, double height, unsigned int depth);
	public:
		Menu_State(StateManager &sM, TextureManager const &textureManager, settings_t &settings);
		virtual ~Menu_State();

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif