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
		std::tuple<std::string, unsigned char const*, size_t>("cursor", ARRAY_WITH_LENGTH(res_cursor_png)),
		std::tuple<std::string, unsigned char const*, size_t>("bullet", ARRAY_WITH_LENGTH(res_bullet_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_01_bg", ARRAY_WITH_LENGTH(res_maps_01_bg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_01_fg", ARRAY_WITH_LENGTH(res_maps_01_fg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_02_bg", ARRAY_WITH_LENGTH(res_maps_02_bg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_02_fg", ARRAY_WITH_LENGTH(res_maps_02_fg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_03_bg", ARRAY_WITH_LENGTH(res_maps_03_bg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_03_fg", ARRAY_WITH_LENGTH(res_maps_03_fg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_04_bg", ARRAY_WITH_LENGTH(res_maps_04_bg_png)),
		std::tuple<std::string, unsigned char const*, size_t>("map_04_fg", ARRAY_WITH_LENGTH(res_maps_04_fg_png)),
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
		{ "cursor", "res/cursor.png" },
		{ "bullet", "res/bullet.png" },
		{ "map_01_bg", "res/maps/01/bg.png"},
		{ "map_01_fg", "res/maps/01/fg.png"},
		{ "map_02_bg", "res/maps/02/bg.png"},
		{ "map_02_fg", "res/maps/02/fg.png"},
		{ "map_03_bg", "res/maps/03/bg.png"},
		{ "map_03_fg", "res/maps/03/fg.png"},
		{ "map_04_bg", "res/maps/04/bg.png"},
		{ "map_04_fg", "res/maps/04/fg.png"},
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
	settings.vsync_on = true;
	settings.music_on = true;
	settings.sound_on = true;
	settings.host = false;

	// Load Textures and Sounds and Shaders
	TextureManager textureManager;
	SoundManager soundManager(settings);
	loadRes(textureManager, soundManager);

	// Create State Manager
	StateManager stateManager(textureManager, soundManager, settings);

	// Create Window
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(desktop, "Matt is a fag.", sf::Style::None);

	window.setPosition(sf::Vector2i(0, 0));
	window.setVerticalSyncEnabled(VSYNC_ON);
	window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);

	settings.videoMode = desktop;

	// View
	sf::Vector2f viewSize(settings.videoMode.width / 2, settings.videoMode.height / 2);
	window.setView(sf::View(sf::Vector2f(viewSize.x / 2, viewSize.y / 2), viewSize));

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