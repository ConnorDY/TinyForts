#include "sound_manager.h"
#include "globals.h"

SoundManager::SoundManager(settings_t const &settings)
	: settings(settings)
{
}

SoundManager::~SoundManager()
{
}


/* Actions */
void SoundManager::loadSound(const std::string &name, const std::string &filename)
{
	/* Add the buffer to the list of buffers */
	auto &buf = buffers[name];

	/* Load the sound buffer */
	buf.loadFromFile(filename);

#ifdef _DEBUG
	std::cout << "Loaded sound \"" << name << "\" from " << filename << std::endl;
#endif

	/* Create sound and add it to the list of sounds */
	this->sounds[name] = std::make_unique<sf::Sound>(buf);
}

void SoundManager::loadSoundFromMemory(const std::string &name, unsigned char const *memory, size_t size)
{
	/* Add the buffer to the list of buffers */
	auto &buf = buffers[name];

	/* Load the sound buffer */
	buf.loadFromMemory(memory, size);

#ifdef _DEBUG
	std::cout << "Loaded sound \"" << name << "\"" << std::endl;
#endif

	/* Create sound and add it to the list of sounds */
	this->sounds[name] = std::make_unique<sf::Sound>(buf);
}

void SoundManager::playSound(const std::string &sound) const
{
	if (!settings.sound_on) return;
	if (sounds.find(sound) != sounds.end()) this->sounds.at(sound)->play();
}