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
	xPos = (wnd->getSize().x / 2) - halfWidth;
	scaife.setPosition(sf::Vector2f(xPos, 100.0f));
	halfWidth = invaders.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	invaders.setPosition(sf::Vector2f(xPos, 165.0f));
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
	_updateDelay = 0.8f;
	float xPos = (Resources::SCREENWIDTH / 2) - (Resources::PLAYERWIDTH / 2);
	_player = std::make_unique<Player>(vec2f(xPos, 550.0f), vec2f(Resources::PLAYERWIDTH, Resources::PLAYERHEIGHT));
	//_entities.emplace_back(std::make_unique<Alien>(vec2f(500.0f, 100.0f), vec2f(Resources::ALIENSIZE, Resources::ALIENSIZE)));
	for (int i = 0; i < 9; i++)
	{
		_aliens.emplace_back(std::make_unique<Alien>(vec2f(100.0f + i * 70.0f, 60.0f), vec2f(Resources::ALIENSIZE, Resources::ALIENSIZE), Resources::AlienType::THIRD));

	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			_aliens.emplace_back(std::make_unique<Alien>(vec2f(100.0f + i * 70.0f, 130.0f + j * 70.0f), vec2f(Resources::ALIENSIZE, Resources::ALIENSIZE), Resources::AlienType::SECOND));
		}		
	}
	for (int i = 0; i < 9; i++)
	{
		_aliens.emplace_back(std::make_unique<Alien>(vec2f(100.0f + i * 70.0f, 270.0f), vec2f(Resources::ALIENSIZE, Resources::ALIENSIZE), Resources::AlienType::FIRST));
	}

}

GameState::~GameState()
{
	_aliens.clear();
	std::cout << "Game State Died" << std::endl;
}

void GameState::Update(float dt)
{	
	if (!_aliens.empty())
	{
		_player->Update(dt);
		static float elapsed = 0.0f;
		elapsed += dt;

		if (elapsed > _updateDelay)
		{
			std::cout << _aliens.size() << std::endl;
			UpdateAliens(dt);
			elapsed = 0.0f;
		}
		for (auto bIt = _bullets.begin(); bIt != _bullets.end();)
		{
			(*bIt)->Update(dt);
			if ((*bIt)->IsOffScreen())
			{
				bIt = _bullets.erase(bIt);
			}
			else
			{
				++bIt;
			}
		}

		if (!_bullets.empty() && !_aliens.empty())
		{
			CheckBulletCollision();
		}

		if (_gameover)
		{
			_gsm->ChangeState(States::GAMEOVER);
		}
	}
	else
	{
		_gsm->ChangeState(States::WIN);
	}
}

void GameState::Draw(sf::RenderWindow* wnd)
{
	std::string s = "Score:  " + std::to_string(_score);
	sf::Text scoreText(s, Resources::getFont("SpaceInvaders"), 20);
	wnd->draw(scoreText);

	_player->Draw(wnd);
	for (auto& a : _aliens)
	{
		a->Draw(wnd);
	}
	for (auto& b : _bullets)
	{
		b->Draw(wnd);
	}
}

void GameState::Input(sf::Event event)
{
	_player->Input(event);
	if (_player->BulletFired())
	{
		vec2f playerPos = _player->GetPos();
		vec2f pos(playerPos.x + Resources::PLAYERWIDTH / 2, playerPos.y);
		_bullets.emplace_back(std::make_unique<Bullet>(pos));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		_gsm->ChangeState(States::TITLE);
	}	
}

void GameState::UpdateAliens(float dt)
{	
	bool velChange = false;
	for (auto it = _aliens.begin(); it != _aliens.end(); ++it)
	{
		//has alien reached the sides of screen?
		if ((*it)->IsOffScreen())
		{
			velChange = true;
			if (_updateDelay > 0.20f)
			{
				_updateDelay *= Resources::DELAYFACTOR;
			}
			break;
		}		
	}


	for (auto it = _aliens.begin(); it != _aliens.end(); ++it)
	{
		if (velChange)
		{
			float xVel = (*it)->GetXVel();
			(*it)->SetVelocity(0.0f, Resources::ALIENYINCREMENT);
			if ((*it)->ReachedBottom())
			{
				_gameover = true;
				break;
			}
			(*it)->Update(dt);
			(*it)->SetVelocity(-xVel, 0.0f);
		}
		else
		{
			(*it)->Update(dt);
		}
	}
}

void GameState::CheckBulletCollision()
{	
	bool collided = false;
	for (auto aIt = _aliens.begin(); aIt != _aliens.end();)
	{
		collided = false;
		for (auto bIt = _bullets.begin(); bIt != _bullets.end();)
		{
			if ((*aIt)->GetBox().contains((*bIt)->GetPos()))
			{
				collided = true;
				bIt = _bullets.erase(bIt);
				break;
			}
			else
			{
				++bIt;
			}
		}
		if (collided)
		{			
			if ((*aIt)->hit())
			{
				_score += (*aIt)->GetScoreValue();
				aIt = _aliens.erase(aIt);
			}			
			break;
		}
		else
		{
			++aIt;
		}
	}
}

GameOverScreen::GameOverScreen(GSM* gsm)
	:
	State(gsm)
{
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::Update(float dt)
{
}

void GameOverScreen::Draw(sf::RenderWindow * wnd)
{
	float halfWidth = 0.0f;
	float xPos = 0.0f;
	sf::Text gameOver("Game Over!", Resources::getFont("SpaceInvaders"), 80);
	sf::Text scaifegoat("You are the\nSCAIFEGOAT", Resources::getFont("SpaceInvaders"), 60);
	sf::Text instruct("Press Q", Resources::getFont("SpaceInvaders"), 20);
	halfWidth = gameOver.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	gameOver.setPosition(sf::Vector2f(xPos, 100.0f));
	halfWidth = scaifegoat.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	scaifegoat.setPosition(sf::Vector2f(xPos, 200.0f));
	halfWidth = instruct.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	instruct.setPosition(sf::Vector2f(xPos, 550.0f));
	wnd->draw(gameOver);
	wnd->draw(scaifegoat);
	wnd->draw(instruct);
}

void GameOverScreen::Input(sf::Event event)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_gsm->ChangeState(States::TITLE);
	}
}


WinScreen::WinScreen(GSM* gsm)
	:
	State(gsm)
{

}

WinScreen::~WinScreen()
{
}

void WinScreen::Update(float dt)
{
}

void WinScreen::Draw(sf::RenderWindow * wnd)
{
	float halfWidth = 0.0f;
	float xPos = 0.0f;
	sf::Text winner("You Win!", Resources::getFont("SpaceInvaders"), 80);
	sf::Text instruct("Press Q", Resources::getFont("SpaceInvaders"), 20);
	halfWidth = winner.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	winner.setPosition(sf::Vector2f(xPos, 100.0f));
	xPos = (wnd->getSize().x / 2) - halfWidth;
	halfWidth = instruct.getLocalBounds().width / 2;
	xPos = (wnd->getSize().x / 2) - halfWidth;
	instruct.setPosition(sf::Vector2f(xPos, 550.0f));
	wnd->draw(winner);
	wnd->draw(instruct);
}

void WinScreen::Input(sf::Event event)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_gsm->ChangeState(States::TITLE);
	}
}
