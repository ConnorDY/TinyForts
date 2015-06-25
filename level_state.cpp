#include "level_state.h"
#include "player.h"
#include "block.h"
#include "globals.h"
#include "menu_state.h"
#include "network_player.h"
#include "network_bullet.h"
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

	view_follow = player;

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

		case 4:
			spawn(new Block(*this, 0, 0, 223, 92));
			spawn(new Block(*this, 0, 92, 7, 159));
			spawn(new Block(*this, 8, 157, 56, 96));
			spawn(new Block(*this, 64, 221, 32, 32));
			spawn(new Block(*this, 0, 252, 31, 132));
			spawn(new Block(*this, 32, 285, 224, 32));
			spawn(new Block(*this, 31, 385, 14, 76));
			spawn(new Block(*this, 126, 317, 34, 64));
			spawn(new Block(*this, 127, 156, 128, 34));
			spawn(new Block(*this, 189, 190, 34, 30));
			spawn(new Block(*this, 256, 156, 352, 18));
			spawn(new Block(*this, 46, 451, 150, 8));
			spawn(new Block(*this, 196, 407, 138, 52));
			spawn(new Block(*this, 334, 432, 150, 26));
			spawn(new Block(*this, 483, 407, 138, 52));
			spawn(new Block(*this, 512, 384, 82, 22));
			spawn(new Block(*this, 543, 355, 32, 30));
			spawn(new Block(*this, 621, 436, 36, 30));
			spawn(new Block(*this, 608, 156, 138, 64));
			spawn(new Block(*this, 746, 156, 33, 32));
			spawn(new Block(*this, 747, 284, 26, 8));
			spawn(new Block(*this, 608, 284, 140, 32));
			spawn(new Block(*this, 621, 436, 36, 30));
			spawn(new Block(*this, 658, 465, 233, 29));
			spawn(new Block(*this, 1609, 90, 7, 68));
			spawn(new Block(*this, 345, 284, 140, 20));
			spawn(new Block(*this, 773, 284, 60, 32));
			spawn(new Block(*this, 805, 316, 74, 64));
			spawn(new Block(*this, 879, 364, 32, 16));
			spawn(new Block(*this, 911, 284, 48, 16));
			spawn(new Block(*this, 959, 284, 150, 32));
			spawn(new Block(*this, 960, 317, 32, 64));
			spawn(new Block(*this, 748, 466, 680, 18));
			spawn(new Block(*this, 894, 412, 138, 54));
			spawn(new Block(*this, 1043, 397, 199, 7));
			spawn(new Block(*this, 1032, 445, 222, 21));
			spawn(new Block(*this, 1168, 283, 165, 20));
			spawn(new Block(*this, 605, 60, 482, 32));
			spawn(new Block(*this, 1069, 92, 18, 16));
			spawn(new Block(*this, 1087, 60, 529, 18));
			spawn(new Block(*this, 776, 156, 216, 32));
			spawn(new Block(*this, 1023, 156, 48, 16));
			spawn(new Block(*this, 959, 188, 33, 48));
			spawn(new Block(*this, 992, 220, 31, 16));
			spawn(new Block(*this, 1055, 172, 14, 64));
			spawn(new Block(*this, 1069, 140, 18, 96));
			spawn(new Block(*this, 1427, 450, 189, 16));
			spawn(new Block(*this, 1571, 384, 14, 66));
			spawn(new Block(*this, 1585, 252, 31, 198));
			spawn(new Block(*this, 1361, 284, 224, 32));
			spawn(new Block(*this, 1457, 316, 34, 64));
			spawn(new Block(*this, 1521, 220, 32, 32));
			spawn(new Block(*this, 1553, 156, 63, 96));
			spawn(new Block(*this, 1361, 156, 128, 34));
			spawn(new Block(*this, 1393, 190, 34, 30));
			spawn(new Block(*this, 1393, 78, 223, 14));
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

	for (unsigned int i = 0; i < otherPlayers.size(); i++)
	{
		network_player p = otherPlayers.at(i);

		p.x += p.dx;
		p.y += p.dy;
		
		player->drawOther(window, p.x, p.y, p.angle, p.frame, p.scale);

		otherPlayers[i] = p;
	}
}

void Level_State::drawForeground(sf::RenderWindow &window)
{
	window.draw(fg);
	cursor.setPosition(getViewX() + mouse.x, getViewY() + mouse.y);
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

	// Mouse
	mouse = sf::Mouse::getPosition();

	mouse.x /= 2;
	mouse.y /= 2;

	// Player movement and arm angle
	sf::Vector2i mouseP = mouse;
	mouseP.x += getViewX();
	mouseP.y += getViewY();

	player->move(moveH, mouseP);

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
		if (player->getClientId() == 0) player->setClientId(client->getSelfId());

		client->update(*this, p);
		otherPlayers = client->getOtherPlayers();
	}
}