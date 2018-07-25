#include "GSM.h"

GSM::GSM()
{
	_states.emplace(std::make_unique<TitleScreen>(this));
}


GSM::~GSM()
{
}

void GSM::Update(float dt)
{
	_states.top()->Update(dt);
}

void GSM::Draw(sf::RenderWindow * wnd)
{
	_states.top()->Draw(wnd);
}

void GSM::Input(sf::Event event)
{
	_states.top()->Input(event);
}

void GSM::ChangeState(States nextState)
{
	switch(nextState)
	{
		case TITLE:
			_states.pop();
			_states.emplace(std::make_unique<TitleScreen>(this));
			break;
		case GAME:
			_states.pop();
			_states.emplace(std::make_unique<GameState>(this));
			break;
		case GAMEOVER:
			_states.pop();
			_states.emplace(std::make_unique<GameOverScreen>(this));
			break;
	}
}
