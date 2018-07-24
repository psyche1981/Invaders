#include "Resources.h"

std::map<std::string, sf::Font> Resources::_fonts;
std::map<std::string, sf::Texture> Resources::_textures;

sf::Texture Resources::loadTexture(const std::string& filename)
{
	sf::Texture tex;
	if (!tex.loadFromFile(filename))
	{
		std::cout << "Texture not loaded: " << filename << std::endl;
	}
	return tex;
}

void Resources::LoadImages()
{
	_textures.insert(std::pair<std::string, sf::Texture>("Nigs1", loadTexture("NS.png")));
}

void Resources::LoadFonts()
{
	_fonts.insert(std::pair<std::string, sf::Font>("SpaceInvaders", loadFont("space_invaders.ttf")));
}

const sf::Font & Resources::getFont(const std::string & name)
{
	return _fonts.at(name);
}

const sf::Texture & Resources::getTexture(const std::string & name)
{
	return _textures.at(name);
}

sf::Font Resources::loadFont(const std::string& filename)
{
	sf::Font f;
	if (!f.loadFromFile(filename))
	{
		std::cout << "Font not loaded: " << filename << std::endl;
	}
	return f;
}