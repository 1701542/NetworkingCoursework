#include "StateMachine.h"

StateMachine::StateMachine(sf::RenderWindow* hwnd, InputManager* input)
{
	window = hwnd;
	inputManager = input;

	menuState = new MenuState(window, inputManager, &currentState);
	gameState = new GameState(window, inputManager, &currentState);
	networkManager = new NetworkManager(gameState);
}

StateMachine::~StateMachine()
{
}

void StateMachine::UpdateStateMachine(float deltaTime)
{
	switch (currentState)
	{
	case(_STATE::MENU) : 
		menuState->Update();
		break;
	case(_STATE::GAME) : 
		gameState->Update(deltaTime);
		networkManager->Update(deltaTime);
		break;
	}
}