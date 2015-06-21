#include "texture_manager.h"
#include "globals.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}


/* Actions */
void TextureManager::loadTexture(const std::string &name, const std::string &filename)
{
	/* Load the texture */
	this->textures[name].loadFromFile(filename);

#ifdef _DEBUG
	std::cout << "Loaded texture \"" << name << "\" from " << filename << std::endl;
#endif

}

void TextureManager::loadTextureFromMemory(const std::string &name, unsigned char const *memory, size_t size)
{
	this->textures[name].loadFromMemory(memory, size);
#ifdef _DEBUG
	std::cout << "Loaded texture \"" << name << "\"" << std::endl;
#endif
}

sf::Texture const& TextureManager::getRef(const std::string &texture) const
{
	static sf::Texture const placeholder;

	auto texture_iter = textures.find(texture);
	if (texture_iter != textures.end()) return texture_iter->second;
	else return placeholder;
}