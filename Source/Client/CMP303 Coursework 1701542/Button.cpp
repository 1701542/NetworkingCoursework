#include "Button.h"



Button::Button(sf::RenderWindow* hwnd)
{
	window = hwnd;

	setSize(sf::Vector2f(200.0f, 100.0f));
	setOrigin(getSize().x / 2.0f, getSize().y / 2.0f);
	setFillColor(sf::Color::Red);
}


Button::~Button()
{
}

void Button::setName(std::string s)
{
	font.loadFromFile("Resources/Font/joystix monospace.ttf");

	nameString = s;
	nameText.setFont(font);
	nameText.setCharacterSize(30.f * (window->getSize().y / 1080.f));
	nameText.setOrigin(sf::Vector2f(((nameString.length() * (nameText.getCharacterSize() * (33.f / 40.f))) / 2.f), ((nameText.getCharacterSize() * (40.f / 33.f)) / 2.f)));
	nameText.setPosition(getPosition().x, getPosition().y);
	nameText.setString(nameString);
}

void Button::Update()
{
	if (highlighted)
	{
		nameText.setFillColor(sf::Color::Black);
	}
	else
	{
		nameText.setFillColor(sf::Color::White);
	}
}