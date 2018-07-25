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


	static constexpr int SCREENWIDTH = 800;
	static constexpr int SCREENHEIGHT = 600;
	static constexpr float PLAYERWIDTH = 75.0f;
	static constexpr float PLAYERHEIGHT = 20.0f;
	static constexpr float PLAYERSPEED = 200.0f;
	static constexpr float ALIENSIZE = 40.0f;
	static constexpr float ALIENSPEED = 25.0f;
	static constexpr float DELAYFACTOR = 0.75f;
	static constexpr float ALIENYINCREMENT = 30.0f;
private:	
	Resources() {}
	~Resources() {}
};


