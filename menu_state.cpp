#include "menu_state.h"

#include "level_state.h"
#include "globals.h"

Menu_State::Menu_State(StateManager &sM, TextureManager const &textureManager, settings_t &settings)
	: State(sM), textureManager(textureManager), settings(settings), bg(sf::Color(70, 163, 178))
{
	// Load font
	if (!fnt.loadFromFile("res/upheaval.ttf")) std::cout << "Failed to load font!" << std::endl;
	const_cast<sf::Texture&>(fnt.getTexture(20)).setSmooth(false);

	// Menu Options
	menuOptions.push_back("Host");
	menuOptions.push_back("Connect");
	menuOptions.push_back("Exit");

	// Reset view
	//window.setView();
}

Menu_State::~Menu_State()
{
	menuOptions.clear();
}


/* Actions */
void Menu_State::drawText(sf::RenderWindow &window, double x, double y, unsigned int size, sf::Color colour, unsigned int alignh, unsigned int alignv, std::string str)
{
	// Create text
	sf::Text text;
	text.setFont(fnt);
	text.setCharacterSize(20 * size);
	text.setString(str);

	// Text colour
	text.setColor(colour);

	// Align text
	sf::FloatRect textRect = text.getLocalBounds();

	double ox = textRect.left, oy = textRect.top;

	switch (alignh)
	{
		case 1:
			ox += textRect.width / 2.0;
			break;

		case 2:
			ox += textRect.width;
			break;
	}

	switch (alignv)
	{
		case 1:
			oy += textRect.height / 2.0;
			break;

		case 2:
			oy += textRect.height;
			break;
	}

	text.setOrigin(round(ox), round(oy));

	// Set position
	text.setPosition(sf::Vector2f(round(x), round(y)));

	// Draw text
	window.draw(text);
}

void Menu_State::drawTab(sf::RenderWindow &window, double x, double y, double width, double height, unsigned int depth)
{
	sf::RectangleShape rect(sf::Vector2f(width, height));
	rect.setFillColor(sf::Color(0, 0, 0, 75 * depth));
	rect.setPosition(x, y);
	window.draw(rect);
}

void Menu_State::draw(sf::RenderWindow &window)
{
	// View
	sf::Vector2f viewSize(settings.videoMode.width / 2, settings.videoMode.height / 2);
	window.setView(sf::View(sf::Vector2f(viewSize.x / 2, viewSize.y / 2), viewSize));

	// BG
	sf::RectangleShape rect(sf::Vector2f(viewSize.x, viewSize.y));
	rect.setFillColor(bg);
	window.draw(rect);

	// Tabs
	drawTab(window, 8, 8, 78, 23, 2);
	drawText(window, 12, 14, 1, sf::Color::White, 0, 0, "Public");

	drawTab(window, 88, 8, 89, 23, 1);
	drawText(window, 92, 14, 1, sf::Color::White, 0, 0, "Private");

	drawTab(window, 179, 8, 126, 23, 1);
	drawText(window, 184, 14, 1, sf::Color::White, 0, 0, "Favourites");

	// Current tab
	drawTab(window, 8, 31, (viewSize.x * .75) - 16, viewSize.y - 39, 2);

	/*double xx = (viewSize.x / 2.0), yy = (viewSize.y / 2.0) - 32.0;

	for (unsigned int i = 0; i < menuOptions.size(); i++)
	{
		// Create text
		sf::Text text;
		text.setFont(fnt);
		text.setCharacterSize(20);
		text.setString(menuOptions.at(i));

		// Text colour
		if (currentOption == i) text.setColor(sf::Color::Yellow);
		else text.setColor(sf::Color::White);

		// Center text
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(round(textRect.left + textRect.width / 2.0f), round(textRect.top + textRect.height / 2.0f));

		// Set position
		text.setPosition(sf::Vector2f(round(xx), round(yy)));
		yy += 32;

		// Draw text
		window.draw(text);
	}*/
}

void Menu_State::update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler)
{
	/*sf::Time deltaTime = */restartClock();

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

				// Host
				case 0:
					settings.host = true;
					getStateManager().setState(std::make_unique<Level_State>(getStateManager(), soundManager, textureManager, settings));
					return;

				// Connect
				case 1:
					settings.host = false;
					getStateManager().setState(std::make_unique<Level_State>(getStateManager(), soundManager, textureManager, settings));
					return;
			}
		}
	}
}