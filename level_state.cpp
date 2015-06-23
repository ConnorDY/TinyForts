#include "level_state.h"
#include "player.h"
#include "block.h"
#include "globals.h"
#include "menu_state.h"
#include "network_player.h"
#include "server.h"
#include "client.h"

Level_State::Level_State(StateManager &sM, SoundManager &som, TextureManager const &textureManager, settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	char buffer[9];

	sprintf(buffer, "map_0%d_bg", MAP);
	bg.setTexture(textureManager.getRef(buffer));

	sprintf(buffer, "map_0%d_fg", MAP);
	fg.setTexture(textureManager.getRef(buffer));

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
	player = new Player(*this, MAP_SPAWN_X, MAP_SPAWN_Y);
	spawn(player);

	switch (MAP)
	{
		case 2:
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
			break;

		case 3:
			spawn(new Block(*this, -4, 300, 260, 12));
			spawn(new Block(*this, 54, 161, 76, 70));
			spawn(new Block(*this, 94, 192, 170, 10));
			spawn(new Block(*this, 182, 244, 74, 10));
			spawn(new Block(*this, 246, 202, 10, 42));
			spawn(new Block(*this, 296, 173, 237, 21));
			spawn(new Block(*this, 320, 194, 10, 60));
			spawn(new Block(*this, 330, 244, 32, 10));
			spawn(new Block(*this, 320, 300, 192, 12));
			spawn(new Block(*this, 410, 244, 12, 56));
			spawn(new Block(*this, 470, 244, 42, 10));
			spawn(new Block(*this, 502, 194, 10, 50));
			spawn(new Block(*this, 568, 192, 138, 10));
			spawn(new Block(*this, 702, 202, 76, 32));
			spawn(new Block(*this, 576, 300, 260, 12));
			spawn(new Block(*this, 576, 202, 10, 52));
			spawn(new Block(*this, 586, 244, 64, 10));
			spawn(new Block(*this, -4, 376, 388, 32));
			spawn(new Block(*this, 448, 376, 388, 32));
			spawn(new Block(*this, 0, 408, 64, 64));
			spawn(new Block(*this, 0, 472, 32, 64));
			spawn(new Block(*this, 32, 504, 64, 32));
			spawn(new Block(*this, 128, 504, 64, 32));
			spawn(new Block(*this, -4, 569, 836, 32));
			spawn(new Block(*this, 160, 440, 64, 32));
			spawn(new Block(*this, 192, 408, 32, 32));
			spawn(new Block(*this, 160, 536, 128, 32));
			spawn(new Block(*this, 256, 472, 320, 32));
			spawn(new Block(*this, 288, 442, 32, 30));
			spawn(new Block(*this, 512, 442, 32, 30));
			spawn(new Block(*this, 608, 408, 32, 64));
			spawn(new Block(*this, 640, 440, 32, 32));
			spawn(new Block(*this, 544, 536, 128, 32));
			spawn(new Block(*this, 640, 504, 64, 32));
			spawn(new Block(*this, 736, 504, 96, 36));
			spawn(new Block(*this, 768, 408, 68, 64));
			spawn(new Block(*this, 800, 472, 32, 32));
			break;
	}

}

Player* Level_State::getPlayer() const
{
	return player;
}

void Level_State::drawBackground(sf::RenderWindow &window)
{
	window.draw(bg);

	int selfId = -1;
	Client *client = getStateManager().getClient();
	if (client != nullptr) selfId = client->getSelfId();

	for (unsigned int i = 0; i < otherPlayers.size(); i++)
	{
		network_player p = otherPlayers.at(i);
		if (p.id == selfId) continue;

		p.x += p.dx;
		p.y += p.dy;
		
		player->drawOther(window, p.x, p.y, p.angle, p.frame, p.scale);

		otherPlayers[i] = p;
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
	Server *server = getStateManager().getServer();
	Client *client = getStateManager().getClient();

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
		{
			network_bullet b = player->shoot();

			if (server != nullptr) server->sendBullet(b);
			if (client != nullptr) client->sendBullet(b);
		}
	}

	mouse = sf::Mouse::getPosition(window);

	player->move(moveH, mouse);

	Room::update(window, soundManager, inputHandler);

	// Networking
	network_player p = player->getNetworkPlayer();

	if (server != nullptr)
	{
		server->update(*this, p);
		otherPlayers = server->getOtherPlayers();
	}

	if (client != nullptr)
	{
		client->update(*this, p);
		otherPlayers = client->getOtherPlayers();
	}
}