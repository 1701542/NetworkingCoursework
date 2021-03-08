#pragma once
#include <SFML\Graphics.hpp>

class Character : public sf::Sprite
{
public:
	Character(sf::RenderWindow* hwnd);
	~Character();

	void Update(float deltaTime, std::vector<int> tileMap);
	void Render();
	sf::FloatRect getCollisionBox() { return sf::FloatRect(collisionBox.left + getPosition().x, collisionBox.top + getPosition().y, collisionBox.width, collisionBox.height); };

private:
	void tileMapCollision(std::vector<int> tileMap);

	sf::RenderWindow* window;

	sf::Sprite playerSprite;
	sf::Texture playerTexture;

	float rotation;
	sf::Vector2f velocity;
	sf::FloatRect collisionBox;
};

