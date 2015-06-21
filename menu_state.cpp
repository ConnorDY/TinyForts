#include "menu_state.h"

#include "level_state.h"
#include "globals.h"

Menu_State::Menu_State(StateManager &sM, TextureManager const &textureManager, settings_t &settings)
	: State(sM), textureManager(textureManager), settings(settings)
{
	// Load font
	if (!fnt.loadFromFile("res/Tewi-normal-11.bdf")) std::cout << "Failed to load font!" << std::endl;
	const_cast<sf::Texture&>(fnt.getTexture(11)).setSmooth(false);

	// Menu Options
	menuOptions.push_back("Start");
	menuOptions.push_back("Exit");
}

Menu_State::~Menu_State()
{
	menuOptions.clear();
}


/* Actions */
void Menu_State::draw(sf::RenderWindow &window)
{
	float xx = 400.0f, yy = 260.0f;

	for (unsigned int i = 0; i < menuOptions.size(); i++)
	{
		// Create text
		sf::Text text;
		text.setFont(fnt);
		text.setCharacterSize(11);
		text.setString(menuOptions.at(i));

		// Text colour
		if (currentOption == i) text.setColor(sf::Color::Yellow);
		else text.setColor(sf::Color::White);

		// Center text
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(round(textRect.left + textRect.width / 2.0f), round(textRect.top + textRect.height / 2.0f));

		// Set position
		text.setPosition(sf::Vector2f(round(xx), round(yy)));
		yy += 16;

		// Draw text
		window.draw(text);
	}
}

void Menu_State::update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler)
{
	sf::Time deltaTime = restartClock();

	// Get Input
	sf::Event event;

	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			default:
				break;

			case sf::Event::Closed:
				window.close();
				break;
		}

		// Escape
		if (inputHandler.checkInput(InputHandler::Input::Exit, event))
		{
			window.close();
			return;
		}

		// Up
		if (inputHandler.checkInput(InputHandler::Input::Up, event))
		{
			if (currentOption == 0) currentOption = menuOptions.size() - 1;
			else currentOption--;
		}

		// Down
		if (inputHandler.checkInput(InputHandler::Input::Down, event))
		{
			if (currentOption == menuOptions.size() - 1) currentOption = 0;
			else currentOption++;
		}

		// Enter
		if (inputHandler.checkInput(InputHandler::Input::Start, event))
		{
			switch (currentOption)
			{
				default:
					std::exit(0);
					return;

				// Load Level State
				case 0:
					getStateManager().setState(std::make_unique<Level_State>(getStateManager(), soundManager, textureManager, settings));
					return;
			}
		}
	}
}