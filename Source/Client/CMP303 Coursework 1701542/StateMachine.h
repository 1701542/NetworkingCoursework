#pragma once
#include "MenuState.h"
#include "GameState.h"
#include "NetworkManager.h"

class StateMachine
{
public:
	StateMachine(sf::RenderWindow* hwnd, InputManager* input);
	~StateMachine();

	void UpdateStateMachine(float deltaTime);

private:

	// States
	_STATE currentState = MENU;
	MenuState* menuState;
	GameState* gameState;
	NetworkManager* networkManager;

	// Pointers
	sf::RenderWindow* window;
	InputManager* inputManager;
};

