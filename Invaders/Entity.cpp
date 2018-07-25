#include "Entity.h"

Entity::Entity(vec2f pos, vec2f size)
	:
	_pos(pos),
	_boundingBox(rect(pos, size)),
	_vel(0.0f, 0.0f),
	_rec(size)
{
	
}

Entity::~Entity()
{

}

const vec2f& Entity::GetPos()
{
	return _pos;
}

Player::Player(vec2f pos, vec2f size)
	:
	Entity(pos,size)
{
	_rec.setFillColor(sf::Color::Red);
}

Player::~Player()
{
	
}

void Player::Update(float dt)
{
	_pos += _vel * dt;
	_rec.setPosition(_pos);
	ConfineToScreen();	
}

void Player::Draw(sf::RenderWindow * wnd)
{	
	wnd->draw(_rec);
}

void Player::Input(sf::Event event)
{	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_vel.x = -Resources::PLAYERSPEED;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_vel.x = Resources::PLAYERSPEED;
	}
	else
	{
		_vel.x = 0.0f;
	}
	_bulletFired = false;
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Space)
		{
			if (_fireBullet)
			{
				_fireBullet = false;
				_bulletFired = true;
			}			
		}
	}

	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Space)
		{
			_fireBullet = true;
		}
	}
	
}

bool Player::BulletFired()
{
	return _bulletFired;
}

void Player::ConfineToScreen()
{
	if (_pos.x < 0.0f)
		_pos.x = 0.0f;
	if (_pos.x > Resources::SCREENWIDTH - _boundingBox.width)
		_pos.x = Resources::SCREENWIDTH - _boundingBox.width;
}

Alien::Alien(vec2f pos, vec2f size)
	:
	Entity(pos, size)
{
	_vel.x = Resources::ALIENSPEED;
	_rec.setPosition(_pos);
	_rec.setFillColor(sf::Color::White);
}

Alien::~Alien()
{
	std::cout << "Alien Died" << std::endl;
}

void Alien::Update(float dt)
{	
	_pos += _vel;	
	_rec.setPosition(_pos);
}

void Alien::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_rec);
}

void Alien::Input(sf::Event event)
{
	//no input for aliens
}

bool Alien::IsOffScreen()
{
	return (_pos.x + _vel.x < 0 || _pos.x + _vel.x > (Resources::SCREENWIDTH - Resources::ALIENSIZE));
}

void Alien::SetVelocity(float xVel, float yPos)
{
	_vel.x = xVel;
	_pos.y += yPos;
}

bool Alien::ReachedBottom()
{
	return _pos.y + Resources::ALIENSIZE > Resources::SCREENHEIGHT;
}

const float Alien::GetXVel()
{
	return _vel.x;
}

Bullet::Bullet(vec2f pos)
	:
	Entity(pos, vec2f(4.0f, 12.0f))
{
	_vel.y = -250.0f;
	_rec.setPosition(_pos);
}

Bullet::~Bullet()
{
	std::cout << "Bullet Died" << std::endl;
}

void Bullet::Update(float dt)
{
	_pos += _vel * dt;
	_rec.setPosition(_pos);
}

void Bullet::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_rec);
}

void Bullet::Input(sf::Event event)
{
	//no input for bullets
}
