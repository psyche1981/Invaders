#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Resources.h"

typedef sf::Vector2f vec2f;
typedef sf::Rect<float> rect;

class Entity
{
public:
	Entity(vec2f pos, vec2f size);
	virtual ~Entity();

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* wnd) = 0;
	virtual void Input(sf::Event event) = 0;

protected:
	vec2f _pos;
	rect _boundingBox;
	vec2f _vel;

	//temp drawing stuff
	sf::RectangleShape _rec;
};


class Bullet : public Entity
{
public:
	Bullet(vec2f pos);
	virtual ~Bullet();
	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;
};

class Player : public Entity
{
public:
	Player(vec2f pos, vec2f size);
	virtual ~Player();


	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

private:
	void ConfineToScreen();
	void BulletFired();
	std::vector<std::unique_ptr<Bullet>> _bullets;

};

class Alien : public Entity
{
public:
	Alien(vec2f pos, vec2f size);
	virtual ~Alien();

	void Update(float dt);
	void Draw(sf::RenderWindow* wnd);
	void Input(sf::Event event);

	bool IsOffScreen();
	void SetVelocity(float xVel, float yPos);
	bool ReachedBottom();
	const float GetXVel();

private:

};
