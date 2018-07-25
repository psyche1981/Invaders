#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <ctime>
#include <random>

#include "Resources.h"
#include "Entity.h"
class GSM;

class State
{
public:
	State(GSM* gsm);
	virtual ~State();

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* wnd) = 0;
	virtual void Input(sf::Event event) = 0;

protected:
	GSM* _gsm;
};

class TitleScreen : public State
{
public:
	TitleScreen(GSM* gsm);
	virtual ~TitleScreen();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

private:
};

class GameState : public State
{
public:
	GameState(GSM* gsm);
	virtual ~GameState();


	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;


private:
	std::unique_ptr<Player> _player;
	std::vector<std::unique_ptr<Alien>> _aliens;
	std::vector<std::unique_ptr<Bullet>> _bullets;
	std::vector<sf::CircleShape> _stars;

	void CreateStarfield();

	float _updateDelay;
	bool _gameover = false;
	int _score = 0;

	void UpdateAliens(float dt);
	void CheckBulletCollision();
	bool CheckPlayerCollision(const Rect& box);
};

class GameOverScreen : public State
{
public:
	GameOverScreen(GSM* gsm);
	virtual ~GameOverScreen();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;

};

class WinScreen : public State
{
public:
	WinScreen(GSM* gsm);
	virtual ~WinScreen();

	void Update(float dt) override;
	void Draw(sf::RenderWindow* wnd) override;
	void Input(sf::Event event) override;
};

