#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include <algorithm>
#include <vector>
#include <memory>

struct settings_t;
class Player;
#include "room.h"

class Level_State : public Room
{
	protected:
		sf::Sprite bg, fg;
		sf::RectangleShape rect;
		Player *player;
	public:
		Level_State(StateManager &sM, SoundManager &som, TextureManager const &textureManager, settings_t &settings);
		virtual ~Level_State();

		Player* getPlayer() const;

		// Actions
		virtual void start() override;
		virtual void drawBackground(sf::RenderWindow &window) override;
		virtual void drawForeground(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif