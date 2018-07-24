#pragma once
#include <SFML/Graphics.hpp>

typedef sf::Vector2f vec2f;
typedef sf::Rect<float> rect;

class Entity
{
public:
	Entity(vec2f pos);
	virtual ~Entity();

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* wnd) = 0;
	virtual void Input(sf::Event event) = 0;

protected:
	vec2f _pos;
	rect _boundingBox;
};

class Player : public Entity
{
public:
	Player();
	virtual ~Player();


	void Update(float dt);
	void Draw(sf::RenderWindow* wnd);
	void Input(sf::Event event);

};