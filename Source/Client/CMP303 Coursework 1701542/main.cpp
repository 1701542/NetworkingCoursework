//--------------------------------
// Client
//--------------------------------

#include "StateMachine.h";
#include "InputManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "1701542 D Frankowski - Client", sf::Style::Close);

	InputManager* inputManager = new InputManager(&window);
	StateMachine* stateManager = new StateMachine(&window, inputManager);

	sf::Clock clock;
	float deltaTime;

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		// Update input manager
		inputManager->UpdateEvents();

		// Update state machine
		stateManager->UpdateStateMachine(deltaTime);
	}

	return 0;
}