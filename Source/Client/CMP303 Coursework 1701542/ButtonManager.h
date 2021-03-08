#pragma once
#include "InputManager.h"
#include "Button.h"
#include "State.h"

class ButtonManager
{
public:
	ButtonManager(sf::RenderWindow* hwnd, InputManager* input, _STATE* state);
	~ButtonManager();

	void MenuInitialiser();

	void Update();
	void Controls();
	void ButtonAssignmentMenu();

	void Render();
	bool IsAButtonHighlighted() { for (int i = 0; i < buttons.size(); i++) { if (buttons[i].getHighlighted()) { return true; } else if (i == buttons.size()) { return false; }; }; };

private:
	sf::RenderWindow* window;
	InputManager* inputManager;
	_STATE* currentState;

	std::vector<Button> buttons;
	int currentButton;
};

