//--------------------------------
// Server
//--------------------------------

#include "NetworkManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "1701542 D Frankowski - Server");

	GameState* gameState = new GameState(&window);
	NetworkManager* networkManager = new NetworkManager(gameState);

	sf::Clock clock;
	float deltaTime;

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		networkManager->Update(deltaTime);

		gameState->Update(deltaTime);
	}

	return 0;
}