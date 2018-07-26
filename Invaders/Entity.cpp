#include "Entity.h"

Entity::Entity(vec2f pos, vec2f size, vec2f vel)
	:
	_pos(pos),
	_boundingBox(Rect(pos, size)),
	_vel(vel),
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

const Rect& Entity::GetBox()
{
	return _boundingBox;
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
	_boundingBox.left = _pos.x;
	_boundingBox.top = _pos.y;
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

bool Player::Hit()
{
	_lives -= 1;
	return _lives == 0;
}

const int Player::GetLives() const
{
	return _lives;
}

void Player::ConfineToScreen()
{
	if (_pos.x < 0.0f)
		_pos.x = 0.0f;
	if (_pos.x > Resources::SCREENWIDTH - _boundingBox.width)
		_pos.x = Resources::SCREENWIDTH - _boundingBox.width;
}

Alien::Alien(vec2f pos, vec2f size, AlienType type)
	:
	Entity(pos, size),
	_type(type)
{
	switch (_type)
	{
	case AlienType::FIRST:
		_hitPoints = 1;
		_scoreValue = 20;
		_sprite = sf::Sprite(Resources::getTexture("Nigs1"));
		break;
	case AlienType::SECOND:
		_hitPoints = 2;
		_scoreValue = 40;
		_sprite = sf::Sprite(Resources::getTexture("Nigs2"));
		break;
	case AlienType::THIRD:
		_hitPoints = 3;
		_scoreValue = 60;
		_sprite = sf::Sprite(Resources::getTexture("Nigs3"));
		break;
	}
	_vel.x = Resources::ALIENSPEED;
	_sprite.setPosition(_boundingBox.left, _boundingBox.top);
	
}

Alien::~Alien()
{
}

void Alien::Update(float dt)
{	
	_pos += _vel;	
	_sprite.setPosition(_pos);
	_boundingBox.left = _pos.x;
	_boundingBox.top = _pos.y;
}

void Alien::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_sprite);
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

const int Alien::GetScoreValue()
{
	return _scoreValue;
}

bool Alien::Hit()
{
	_hitPoints -= 1;
	return _hitPoints == 0;
}

void Alien::Shoot()
{
	std::cout << "X: " << _pos.x << "; Y: " << _pos.y << std::endl;

}

Bullet::Bullet(vec2f pos, vec2f vel)
	:
	Entity(pos, vec2f(4.0f, 12.0f), vel)
{
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
	_boundingBox.left = _pos.x;
	_boundingBox.top = _pos.y;
}

void Bullet::Draw(sf::RenderWindow * wnd)
{
	wnd->draw(_rec);
}

void Bullet::Input(sf::Event event)
{
	//no input for bullets
}

bool Bullet::IsOffScreen()
{
	return _pos.y < 0 || _pos.y > Resources::SCREENHEIGHT;
}
