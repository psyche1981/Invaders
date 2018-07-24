#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <string>


class Resources
{
public:	
	static void LoadImages();
	static void LoadFonts();


	static const sf::Font& getFont(const std::string& name);
	static const sf::Texture& getTexture(const std::string& name);


	static std::map<std::string, sf::Font> _fonts;
	static std::map<std::string, sf::Texture> _textures;

	static sf::Texture loadTexture(const std::string& filename);
	static sf::Font loadFont(const std::string& filename);
private:	
	Resources() {}
	~Resources() {}
};


