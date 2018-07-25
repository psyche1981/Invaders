#include "State.h"
#include "GSM.h"

State::State(GSM* gsm)
	:
	_gsm(gsm)
{
}


State::~State()
{
}

TitleScreen::TitleScreen(GSM* gsm)
	:
	State(gsm)
{
}

TitleScreen::~TitleScreen()
{
	
}

void TitleScreen::Update(float dt)
{
}

void TitleScreen::Draw(sf::RenderWindow * wnd)
{
	float halfWidth = 0.0f;
	float xPos = 0.0f;
	sf::Text scaife("SCAIFE", Resources::getFont("SpaceInvaders"), 60);	
	sf::Text invaders("INVADERS", Resources::getFont("SpaceInvaders"), 60);
	sf::Text instruct("Press Space Bar", Resources::getFont("SpaceInvaders"), 20);
	halfWidth = scaife.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2 )- halfWidth;
	scaife.setPosition(sf::Vector2f(xPos, 100.0f));
	halfWidth = invaders.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	invaders.setPosition(sf::Vector2f(xPos, 160.0f));
	halfWidth = instruct.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	instruct.setPosition(sf::Vector2f(xPos, 400.0f));
	wnd->draw(scaife);
	wnd->draw(invaders);
	wnd->draw(instruct);
}

void TitleScreen::Input(sf::Event event)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		_gsm->ChangeState(States::GAME);
	}

}

GameState::GameState(GSM* gsm)
	:
	State(gsm)
{
	float xPos = (Resources::SCREENWIDTH / 2) - (Resources::PLAYERWIDTH / 2);
	_entities.emplace_back(std::make_unique<Player>(vec2f(xPos, 550.0f), vec2f(Resources::PLAYERWIDTH, Resources::PLAYERHEIGHT)));
	_entities.emplace_back(std::make_unique<Alien>(vec2f(100.0f, 100.0f), vec2f(Resources::ALIENSIZE, Resources::ALIENSIZE)));
}

GameState::~GameState()
{
	_entities.clear();
	std::cout << "Game State Died" << std::endl;
}

void GameState::Update(float dt)
{
	for (auto it = _entities.begin(); it != _entities.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void GameState::Draw(sf::RenderWindow* wnd)
{
	sf::Text scoreText("Score: ", Resources::getFont("SpaceInvaders"), 20);
	wnd->draw(scoreText);


	for (auto it = _entities.begin(); it != _entities.end(); ++it)
	{
		(*it)->Draw(wnd);
	}
}

void GameState::Input(sf::Event event)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		_gsm->ChangeState(States::TITLE);
	}
	if (!_entities.empty())
	{
		_entities[0]->Input(event);
	}	
}
