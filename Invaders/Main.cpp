#include <iostream>
#include <chrono>
#include<SFML/Graphics.hpp>

#include "GSM.h"
#include "Resources.h"

class Window
{
public:
	Window(int width, int height, const std::string& title)
		:
		_window(sf::VideoMode(width, height), title),
		_gsm()
	{
		Resources::LoadFonts();
		Resources::LoadImages();
	}

	virtual ~Window() {}

	void Loop()
	{
		auto tp = std::chrono::steady_clock::now();
		sf::Event event;

		while (_window.isOpen())
		{

			while (_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					_window.close();

				_gsm.Input(event);
			}

			float dt;
			{
				const auto new_tp = std::chrono::steady_clock::now();
				dt = std::chrono::duration<float>(new_tp - tp).count();
				tp = new_tp;
			}

			_window.clear();



			_gsm.Update(dt);

			_gsm.Draw(&_window);

			_window.display();
		}
	}

private:
	sf::RenderWindow _window;
	GSM _gsm;
};

int main()
{
	Window wnd(Resources::SCREENWIDTH, Resources::SCREENHEIGHT, "Space Invaders");
	wnd.Loop();
	return 0;
}