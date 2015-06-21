#include <SFML/Graphics.hpp>
#include <vector>

#include "globals.h"
#include "settings.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "state_manager.h"

#ifdef STATIC_RESOURCES
#include "static_resource.h"
#endif

/* Load Game's Resources */
void loadRes(TextureManager &textureManager, SoundManager &soundManager)
{
	// TODO:
	// This is a clusterfuck, we can probably do this better.
	// Either we generate an include file that does this (we'll need a text file for mapping name to file in that case)
	// Or we use macros to expand to the right one e.g. RES("player-dead", res_plyr_plyr_dead_png, "res/plyr/plyr_dead.png")
	// Keeping this for now though because there's no real reason to work on it so close to the deadline for demo
#ifdef STATIC_RESOURCES
	auto textures = std::vector<std::tuple<std::string, unsigned char const*, size_t>>
	{
		std::tuple<std::string, unsigned char const*, size_t>("player", ARRAY_WITH_LENGTH(res_player_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_01_bg", ARRAY_WITH_LENGTH(res_maps_01_bg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_01_fg", ARRAY_WITH_LENGTH(res_maps_01_fg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_02_bg", ARRAY_WITH_LENGTH(res_maps_02_bg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_02_fg", ARRAY_WITH_LENGTH(res_maps_02_fg_png)),
	};

	auto sounds = std::vector<std::tuple<std::string, unsigned char const*, size_t>>
	{

	};

	for (auto &texture : textures) textureManager.loadTextureFromMemory(std::get<0>(texture), std::get<1>(texture), std::get<2>(texture));
	for (auto &sound : sounds) soundManager.loadSoundFromMemory(std::get<0>(sound), std::get<1>(sound), std::get<2>(sound));
#else
	auto textures = std::vector<std::pair<std::string, std::string>>
	{
		{ "player", "res/player.png" },
		{ "map_01_bg", "res/maps/01/bg.png"},
		{ "map_01_fg", "res/maps/01/fg.png"},
		{ "map_02_bg", "res/maps/02/bg.png"},
		{ "map_02_fg", "res/maps/02/fg.png"},
	};

	auto sounds = std::vector<std::pair<std::string, std::string>>
	{
		
	};

	for (auto &texture : textures) textureManager.loadTexture(texture.first, texture.second);
	for (auto &sound : sounds) soundManager.loadSound(sound.first, sound.second);
#endif
}


/* Program Entry Point */
int main()
{
	// Default settings
	settings_t settings;
	settings.window_scale = 2;
	settings.vsync_on = true;
	settings.music_on = true;
	settings.sound_on = true;

	// Load Textures and Sounds and Shaders
	TextureManager textureManager;
	SoundManager soundManager(settings);
	loadRes(textureManager, soundManager);

	// Create State Manager
	StateManager stateManager(textureManager, soundManager, settings);

	// Create Window
	sf::RenderWindow window(sf::VideoMode(ROOM_WIDTH, ROOM_HEIGHT), "Matt is a fag.", sf::Style::Close | sf::Style::Titlebar);
	window.setVerticalSyncEnabled(VSYNC_ON);

	// Game Loop
	while (window.isOpen())
	{
		// Update State
		stateManager.update(window);

		// Draw the State
		window.clear();
		stateManager.draw(window);
		window.display();
	}

	return 0;
}