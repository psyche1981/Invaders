#include "Entity.h"

Entity::Entity(vec2f pos)
	:
	_pos(pos)
{
	_boundingBox = rect(_pos, vec2f(50.0f, 50.0f));
}

Entity::~Entity()
{

}

Player::Player()
	:
	Entity(vec2f(100.0f, 100.0f))
{
}

Player::~Player()
{
}

void Player::Update(float dt)
{
}

void Player::Draw(sf::RenderWindow * wnd)
{
	vec2f size(_boundingBox.width, _boundingBox.height);
	sf::RectangleShape rec(size);
	rec.setPosition(_pos);
	rec.setFillColor(sf::Color(255, 200, 100));
	wnd->draw(rec);
}

void Player::Input(sf::Event event)
{
}
