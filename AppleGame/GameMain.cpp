#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"

int main()
{
	using namespace ApplesGame;

	int seed = (int)time(nullptr);
	srand(seed);

	sf::RenderWindow window(
		sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
		"Apples game!"
	);

	Game game;
	game.mod = Modes::Mods_1; 
	InitGame(game);

	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		sf::sleep(sf::milliseconds(16));

		float currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
					break;
				}

				if (event.key.code == sf::Keyboard::Num1)
				{
					game.mod = Modes::Mods_1;
					DeinializeGame(game);
					InitGame(game);
				}

				if (event.key.code == sf::Keyboard::Num2)
				{
					game.mod = Modes::Mods_2;
					DeinializeGame(game);
					InitGame(game);
				}

				if (event.key.code == sf::Keyboard::Num3)
				{
					game.mod = Modes::Mods_3;
					DeinializeGame(game);
					InitGame(game);
				}
			}
		}

		UpdateGame(game, deltaTime);

		window.clear();
		DrawGame(game, window);
		window.display();
	}

	DeinializeGame(game);

	return 0;
}
