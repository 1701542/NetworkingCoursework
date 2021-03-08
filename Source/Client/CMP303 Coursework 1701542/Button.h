#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class Button : public sf::RectangleShape
{
public:
	Button(sf::RenderWindow* hwnd);
	~Button();

	void Update();

	void setHighlighted(bool b) { highlighted = b; };
	bool getHighlighted() { return highlighted; };
	void setName(std::string s);
	sf::Text getName() { return nameText; };

	sf::FloatRect collisionBox;
private:
	sf::RenderWindow* window;

	sf::Font font;
	std::string nameString;
	sf::Text nameText;
	bool highlighted;
};

