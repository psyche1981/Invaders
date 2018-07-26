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
	_randEngine = std::mt19937(time(nullptr));
	_updateDelay = 0.8f;
	float xPos = (Resources::SCREENWIDTH / 2) - (Resources::PLAYERWIDTH / 2);
	_player = std::make_unique<Player>(vec2f(xPos, 550.0f), vec2f(Resources::PLAYERWIDTH, Resources::PLAYERHEIGHT));
	for (int i = 0; i < 9; i++)
	{
		_aliens.emplace_back(std::make_unique<Alien>(vec2f(100.0f + i * 70.0f, 60.0f), vec2f(Resources::ALIENSIZE, Resources::ALIENSIZE), AlienType::THIRD));

	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			_aliens.emplace_back(std::make_unique<Alien>(vec2f(100.0f + i * 70.0f, 130.0f + j * 70.0f), vec2f(Resources::ALIENSIZE, Resources::ALIENSIZE), AlienType::SECOND));
		}		
	}
	for (int i = 0; i < 9; i++)
	{
		_aliens.emplace_back(std::make_unique<Alien>(vec2f(100.0f + i * 70.0f, 270.0f), vec2f(34.0f, 41.0f), AlienType::FIRST));
	}
	CreateStarfield();
}

GameState::~GameState()
{
	_aliens.clear();
	
}

void GameState::CreateStarfield()
{
	std::uniform_int_distribution<int> randXPick(0,Resources::SCREENWIDTH);
	std::uniform_int_distribution<int> randYPick(0, Resources::SCREENHEIGHT);
	std::uniform_int_distribution<int> randSizePick(1, 100);
	for (int i = 0; i < Resources::NUMSTARS; i++)
	{	
		float xPos = randXPick(_randEngine);
		float yPos = randYPick(_randEngine);
		int sizeRoll = randSizePick(_randEngine);
		sf::Color colour;
		float size = 0;
		if (sizeRoll > 95)
		{
			size = 2.0f;
			colour = sf::Color(255, 255, 255, 150);
		}
		else if (sizeRoll > 75)
		{
			size = 1.5f;
			colour = sf::Color(255, 255, 255, 175);
		}
		else
		{
			size = 1.0f;
			colour = sf::Color(255, 255, 255, 200);
		}
		sf::CircleShape star(size);
		star.setPosition(xPos, yPos);
		star.setFillColor(colour);
		_stars.push_back(star);
	}
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
		for (auto bIt = _alienBullets.begin(); bIt != _alienBullets.end();)
		{
			(*bIt)->Update(dt);
			if ((*bIt)->IsOffScreen())
			{
				bIt = _alienBullets.erase(bIt);
			}
			else
			{
				++bIt;
			}
		}

		if (!_bullets.empty())
		{
			CheckBulletAlienCollision();
		}
		if (!_alienBullets.empty())
		{
			CheckBulletPlayerCollision();
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
	for (auto& s : _stars)
	{
		wnd->draw(s);
	}

	std::string s = "Score:  " + std::to_string(_score);
	sf::Text scoreText(s, Resources::getFont("SpaceInvaders"), 20);
	wnd->draw(scoreText);
	s = "Lives: " + std::to_string(_player->GetLives());
	sf::Text livesText(s, Resources::getFont("SpaceInvaders"), 20);
	livesText.setPosition(650.0f, 0.0f);
	wnd->draw(livesText);
	for (auto& b : _bullets)
	{
		b->Draw(wnd);
	}
	for (auto& ab : _alienBullets)
	{
		ab->Draw(wnd);
	}

	_player->Draw(wnd);
	for (auto& a : _aliens)
	{
		a->Draw(wnd);
	}	
}

void GameState::Input(sf::Event event)
{
	_player->Input(event);
	if (_player->BulletFired())
	{
		vec2f playerPos = _player->GetPos();
		vec2f pos(playerPos.x + Resources::PLAYERWIDTH / 2, playerPos.y);
		_bullets.emplace_back(std::make_unique<Bullet>(pos, vec2f(0.0f, -Resources::BULLETSPEED)));
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
		if ((*it)->GetPos().y > lowestY)
		{
			lowestY = (*it)->GetPos().y;
		}
		if (CheckPlayerAlienCollision((*it)->GetBox()))
		{
			_gameover = true;
		}
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

	std::uniform_int_distribution<int> randAlienPick(0, _aliens.size() -1);
	std::unique_ptr<Alien>& randAlien = _aliens[randAlienPick(_randEngine)];
	if (randAlien->GetPos().y == lowestY)
	{
		std::cout << "low Y: " <<lowestY << std::endl;
		randAlien->Shoot();
		vec2f alienPos = randAlien->GetPos();
		vec2f pos(alienPos.x + Resources::ALIENSIZE / 2, alienPos.y + randAlien->GetBox().height);
		_alienBullets.emplace_back(std::make_unique<Bullet>(pos, vec2f(0.0f, Resources::BULLETSPEED)));
	}

	if (!_gameover)
	{
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
}

void GameState::CheckBulletAlienCollision()
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
			if ((*aIt)->Hit())
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

bool GameState::CheckPlayerAlienCollision(const Rect& box)
{
	vec2f pos = _player->GetPos();
	float width = _player->GetBox().width;
	return (box.top + box.height > pos.y) && (box.left > pos.x) && box.left < pos.x + width;
}

void GameState::CheckBulletPlayerCollision()
{
	bool collided = false;
	for (auto bIt = _alienBullets.begin(); bIt != _alienBullets.end();)
	{
		collided = false;
		if (_player->GetBox().contains((*bIt)->GetPos()))
		{
			collided = true;
			bIt = _alienBullets.erase(bIt);
			if (_player->Hit())
			{
				_gameover = true;
			}
		}
		else
		{
			++bIt;
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
