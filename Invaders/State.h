#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

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
	std::vector<std::unique_ptr<Entity>> _entities;
};

