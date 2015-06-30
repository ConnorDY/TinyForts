#include "menu_state.h"

#include "level_state.h"
#include "globals.h"

Menu_State::Menu_State(StateManager &sM, TextureManager const &textureManager, settings_t &settings)
	: State(sM), textureManager(textureManager), settings(settings), bg(sf::Color(70, 163, 178)),
	currentTab(0)
{
	// Load font
	if (!fnt.loadFromFile("res/upheaval.ttf")) std::cout << "Failed to load font!" << std::endl;
	const_cast<sf::Texture&>(fnt.getTexture(20)).setSmooth(false);
	const_cast<sf::Texture&>(fnt.getTexture(40)).setSmooth(false);

	cursor.setTexture(textureManager.getRef("cursor"));
	cursor.setOrigin(10, 10);
}

Menu_State::~Menu_State()
{
	
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
	drawTab(window, 8, 8, 78, 23, 1 + (currentTab == 0));
	drawText(window, 12, 14, 1, sf::Color::White, 0, 0, "Public");

	drawTab(window, 88, 8, 89, 23, 1 + (currentTab == 1));
	drawText(window, 92, 14, 1, sf::Color::White, 0, 0, "Private");

	drawTab(window, 179, 8, 126, 23, 1 + (currentTab == 2));
	drawText(window, 184, 14, 1, sf::Color::White, 0, 0, "Favourites");

	// Current tab
	drawTab(window, 8, 31, viewSize.x - 236, viewSize.y - 39, 2);

	drawTab(window, 18, 41, viewSize.x - 256, viewSize.y - 89, 1);

	drawTab(window, viewSize.x - 330, viewSize.y - 38, 92, 21, 1);
	drawText(window, viewSize.x - 243, viewSize.y - 23, 1, sf::Color::White, 2, 2, "Connect");

	// Cursor
	cursor.setPosition(getViewX() + mouse.x, getViewY() + mouse.y);
	window.draw(cursor);
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

		// Left click
		if (inputHandler.checkInput(InputHandler::Input::PressL, event))
		{
			if (mouse.y >= 8 && mouse.y < 31)
			{
				if (mouse.x >= 8 && mouse.x < 86) currentTab = 0;
				else if (mouse.x >= 88 && mouse.x < 177) currentTab = 1;
				else if (mouse.x >= 179 && mouse.x < 305) currentTab = 2;
			}
		}
	}

	// Mouse
	mouse = sf::Mouse::getPosition();

	mouse.x /= 2;
	mouse.y /= 2;
}