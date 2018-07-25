#pragma once
#include <SFML\Graphics.hpp>
#include <stack>
#include <memory>

#include "State.h"

enum States
{
	TITLE,
	GAME,
	GAMEOVER,
	WIN
};

class GSM
{
public:
	GSM();
	~GSM();

	void Update(float dt);
	void Draw(sf::RenderWindow* wnd);
	void Input(sf::Event event);

	void ChangeState(States nextState);
	const size_t GetStackSize();

private:
	std::stack<std::unique_ptr<State>> _states;
};

