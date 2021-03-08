#pragma once
#include "State.h"
#include "ButtonManager.h"

class MenuState : protected State
{
public:
	MenuState(sf::RenderWindow* hwnd, InputManager* input, _STATE* state);
	~MenuState();

	void Update();

private:
	void UpdateInput();
	void Render();

	sf::RenderWindow* window;
	InputManager* inputManager;
	_STATE* currentState;

	ButtonManager* buttonManager;
};

