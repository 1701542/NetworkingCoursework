#include "MenuState.h"



MenuState::MenuState(sf::RenderWindow* hwnd, InputManager* input, _STATE* state)
{
	window = hwnd;
	inputManager = input;
	currentState = state;

	buttonManager = new ButtonManager(window, inputManager, currentState);
	buttonManager->MenuInitialiser();
}


MenuState::~MenuState()
{
}

void MenuState::Update()
{
	UpdateInput();

	buttonManager->Update();

	Render();
}

void MenuState::UpdateInput()
{
	buttonManager->Controls();
	buttonManager->ButtonAssignmentMenu();

	if (inputManager->getKey(sf::Keyboard::Enter))
	{
		*currentState = _STATE::GAME;
		inputManager->setKey(sf::Keyboard::Enter, false);
	}
}

void MenuState::Render()
{
	window->clear(sf::Color(125, 10, 10, 255));

	buttonManager->Render();

	window->display();
}