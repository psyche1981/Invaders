#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Resources.h"

typedef sf::Vector2f vec2f;
typedef sf::Rect<float> Rect;

enum AlienType
{
	FIRST,
	SECOND,
	THIRD
};

class Entity
{
public:
	Entity(vec2f pos, vec2f size, vec2f vel = vec2f(0.0f, 0.0f));
	virtual ~Entity();

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* wnd) = 0;
	virtual void Input(sf::Event event) = 0;

	const vec2f& GetPos();
	const Rect& GetBox();

protected:
	vec2f _pos;
	Rect _boundingBox;
	vec2f _vel;

	sf::Sprite _sprite;

	//temp drawing stuff
	sf::RectangleShape _rec;
};


class Bullet : public Entity
{
public:
	Bullet(vec2f pos, vec2f vel);
	virtual ~Bullet();
	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

	bool IsOffScreen();

};

class Player : public Entity
{
public:
	Player(vec2f pos, vec2f size);
	virtual ~Player();


	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

	bool BulletFired();
	bool Hit();
	const int GetLives() const;

private:
	void ConfineToScreen();

	bool _fireBullet = true;
	bool _bulletFired = false;
	int _lives = 3;

};

class Alien : public Entity
{
public:
	Alien(vec2f pos, vec2f size, AlienType type);
	virtual ~Alien();

	void Update(float dt);
	void Draw(sf::RenderWindow* wnd);
	void Input(sf::Event event);

	bool IsOffScreen();
	void SetVelocity(float xVel, float yPos);
	bool ReachedBottom();
	const float GetXVel();
	const int GetScoreValue();

	bool Hit();
	void Shoot();

private:
	AlienType _type;
	int _scoreValue;
	int _hitPoints;
};
