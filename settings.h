#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>

struct settings_t
{
	bool vsync_on, music_on, sound_on, host;
	unsigned int player_skin;
	sf::VideoMode videoMode;
};

#endif