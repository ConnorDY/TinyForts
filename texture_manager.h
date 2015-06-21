#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <map>

class TextureManager
{
	private:
		std::map<std::string, sf::Texture> textures;
	public:
		TextureManager();
		~TextureManager();

		// Actions
		void loadTexture(const std::string &name, const std::string &filename);
		void loadTextureFromMemory(const std::string &name, unsigned char const *memory, size_t size);
		sf::Texture const& getRef(const std::string &texture) const;
};

#endif