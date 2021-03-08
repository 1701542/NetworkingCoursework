#include "ButtonManager.h"



ButtonManager::ButtonManager(sf::RenderWindow* hwnd, InputManager* input, _STATE* state)
{
	window = hwnd;
	inputManager = input;
	currentState = state;
}


ButtonManager::~ButtonManager()
{
}

void ButtonManager::MenuInitialiser()
{

	for (int i = 0; i < 2; i++)
	{
		Button* button = new Button(window);
		button->setPosition(window->getSize().x / 2, (window->getSize().y / 2.0f) + (128 * i));
		button->collisionBox = sf::FloatRect(button->getPosition().x - (button->getSize().x / 2.0f), button->getPosition().y - (button->getSize().y / 2.0f), button->getSize().x, button->getSize().y);//sf::FloatRect(button->getTextureRect());
		buttons.push_back(*button);
		delete button;
	}
	buttons[0].setName("Play Game");
	buttons[1].setName("Quit");
}

void ButtonManager::Update()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].Update();

		if (i == currentButton)
		{
			buttons[i].setHighlighted(true);
		}
		else
		{
			buttons[i].setHighlighted(false);
		}

		if (buttons[i].getHighlighted())
		{
			buttons[i].setFillColor(sf::Color(0, 0, 255, 255));
		}
		else
		{
			buttons[i].setFillColor(sf::Color(255, 0, 0, 255));
		}
	}
}

void ButtonManager::Controls()
{
	if (inputManager->getKey(sf::Keyboard::S) ||
		inputManager->getKey(sf::Keyboard::Down) ||
		inputManager->getKey(sf::Keyboard::D) ||
		inputManager->getKey(sf::Keyboard::Right))
	{
		if (currentButton == buttons.size() || !IsAButtonHighlighted())
		{
			currentButton = 0;
		}
		else
		{
			currentButton++;
		}
		inputManager->setKey(sf::Keyboard::S, false);
		inputManager->setKey(sf::Keyboard::Down, false);
		inputManager->setKey(sf::Keyboard::D, false);
		inputManager->setKey(sf::Keyboard::Right, false);
	}
	else if (inputManager->getKey(sf::Keyboard::W) ||
		inputManager->getKey(sf::Keyboard::Up) ||
		inputManager->getKey(sf::Keyboard::A) ||
		inputManager->getKey(sf::Keyboard::Left))
	{
		if (currentButton == 0 || !IsAButtonHighlighted())
		{
			currentButton = buttons.size();
		}
		else
		{
			currentButton--;
		}
		inputManager->setKey(sf::Keyboard::W, false);
		inputManager->setKey(sf::Keyboard::Up, false);
		inputManager->setKey(sf::Keyboard::A, false);
		inputManager->setKey(sf::Keyboard::Left, false);
	}
	for (int i = 0; i < buttons.size(); i++)
	{
		if (inputManager->getMouseX() > buttons[i].collisionBox.left &&
			inputManager->getMouseX() < buttons[i].collisionBox.width + buttons[i].collisionBox.left &&
			inputManager->getMouseY() > buttons[i].collisionBox.top &&
			inputManager->getMouseY() < buttons[i].collisionBox.height + buttons[i].collisionBox.top)
		{
			if (inputManager->getMouseLeft())
			{
				inputManager->setKey(sf::Keyboard::Space, true);
			}
			currentButton = i;
		}
	}
}

void ButtonManager::ButtonAssignmentMenu()
{
	if (inputManager->getKey(sf::Keyboard::Space) ||
		inputManager->getKey(sf::Keyboard::Return))
	{
		if (buttons[0].getHighlighted())
		{
			*currentState = _STATE::GAME;
			inputManager->setKey(sf::Keyboard::Space, false);
			inputManager->setKey(sf::Keyboard::Return, false);
		}
		if (buttons[1].getHighlighted())
		{
			window->close();
		}
	}
}

void ButtonManager::Render()
{
	for (int i = 0; i < buttons.size(); i++)
	{
		window->draw(buttons[i]);
		window->draw(buttons[i].getName());
	}
}