#include "level_state.h"
#include "player.h"
#include "block.h"
#include "globals.h"
#include "menu_state.h"
#include "server.h"
#include "client.h"

Level_State::Level_State(StateManager &sM, SoundManager &som, TextureManager const &textureManager, settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	bg.setTexture(textureManager.getRef("map_02_bg"));
	fg.setTexture(textureManager.getRef("map_02_fg"));

	cursor.setTexture(textureManager.getRef("cursor"));
	cursor.setOrigin(10, 10);

	rect.setSize(sf::Vector2f(VIEW_WIDTH, BAR_HEIGHT));
	rect.setFillColor(sf::Color::Black);

	if (settings.host) sM.createServer();
	else sM.createClient();

	start();
}

Level_State::~Level_State()
{
}

void Level_State::start()
{
	player = new Player(*this, 300, 200);
	spawn(player);

	spawn(new Block(*this, 160, 87, 224, 33));
	spawn(new Block(*this, 368, 120, 16, 33));
	spawn(new Block(*this, 448, 87, 224, 33));
	spawn(new Block(*this, 448, 120, 16, 33));
	spawn(new Block(*this, 160, 184, 512, 32));
	spawn(new Block(*this, 256, 120, 32, 32));
	spawn(new Block(*this, 544, 120, 32, 32));
	spawn(new Block(*this, -4, 184, 68, 32));
	spawn(new Block(*this, 768, 184, 68, 32));
	spawn(new Block(*this, 12, 264, 148, 18));
	spawn(new Block(*this, 64, 282, 96, 14));
	spawn(new Block(*this, 672, 264, 148, 18));
	spawn(new Block(*this, 672, 282, 96, 14));
	spawn(new Block(*this, -4, 360, 230, 16));
	spawn(new Block(*this, 64, 376, 162, 16));
	spawn(new Block(*this, 606, 360, 230, 16));
	spawn(new Block(*this, 606, 376, 162, 16));
	spawn(new Block(*this, 224, 296, 96, 32));
	spawn(new Block(*this, 288, 328, 32, 64));
	spawn(new Block(*this, 320, 360, 224, 32));
	spawn(new Block(*this, 384, 280, 64, 38));
	spawn(new Block(*this, 512, 296, 32, 96));
	spawn(new Block(*this, 544, 296, 64, 32));
	spawn(new Block(*this, 160, 424, 32, 16));
	spawn(new Block(*this, 224, 424, 384, 16));
	spawn(new Block(*this, 271, 40, 290, 16));
	spawn(new Block(*this, 640, 424, 32, 16));
	spawn(new Block(*this, 369, 216, 94, 16));
	spawn(new Block(*this, 176, 474, 16, 110));
	spawn(new Block(*this, 224, 474, 16, 62));
	spawn(new Block(*this, 192, 568, 192, 16));
	spawn(new Block(*this, 240, 520, 352, 16));
	spawn(new Block(*this, 592, 474, 16, 62));
	spawn(new Block(*this, 368, 584, 16, 16));
	spawn(new Block(*this, 448, 568, 16, 32));
	spawn(new Block(*this, 448, 568, 192, 16));
	spawn(new Block(*this, 640,474, 16, 94));
	spawn(new Block(*this, -4, 87, 164, 16));
	spawn(new Block(*this, 673, 87, 164, 16));

}

Player* Level_State::getPlayer() const
{
	return player;
}

void Level_State::drawBackground(sf::RenderWindow &window)
{
	window.draw(bg);

	for (network_player p : otherPlayers)
	{
		player->drawOther(window, p.x, p.y, p.angle, p.frame, p.scale);
	}
}

void Level_State::drawForeground(sf::RenderWindow &window)
{
	window.draw(fg);
	window.draw(rect);
	cursor.setPosition(mouse.x, mouse.y);
	window.draw(cursor);
}

void Level_State::update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler)
{
	int moveH = inputHandler.checkInput(InputHandler::Input::Right) - inputHandler.checkInput(InputHandler::Input::Left); // Horizontal Movement

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

		if (inputHandler.checkInput(InputHandler::Input::Exit, event))
		{
			getStateManager().setState(std::make_unique<Menu_State>(getStateManager(), textureManager, settings));
			return;
		}

		if (inputHandler.checkInput(InputHandler::Input::Up, event))
			player->jump(); // Jumping

		if (inputHandler.checkInput(InputHandler::Input::PressL, event))
			player->shoot();
	}

	mouse = sf::Mouse::getPosition(window);

	player->move(moveH, mouse);

	Room::update(window, soundManager, inputHandler);

	// Networking
	Server *server = getStateManager().getServer();
	Client *client = getStateManager().getClient();

	if (server != nullptr)
	{
		server->update();
		otherPlayers = server->getOtherPlayers();
	}

	if (client != nullptr)
	{
		network_player p;
		p.x = player->getX();
		p.y = player->getY();
		p.angle = player->getAngle();
		p.frame = floor(player->getFrame());
		p.scale = player->getScale();

		client->update(p);
	}
}