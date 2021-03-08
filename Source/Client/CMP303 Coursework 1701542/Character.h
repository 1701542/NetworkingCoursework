#pragma once
#include <SFML\Graphics.hpp>
#include "InputManager.h"

class Character : public sf::Sprite
{
public:
	Character(sf::RenderWindow* hwnd, InputManager* input);
	~Character();

	void Update(float deltaTime, std::vector<int> tileMap);
	void UpdateInput();
	void Render();
	sf::FloatRect getCollisionBox() { return sf::FloatRect(collisionBox.left + getPosition().x, collisionBox.top + getPosition().y, collisionBox.width, collisionBox.height); };

private:
	void tileMapCollision(std::vector<int> tileMap);

	sf::RenderWindow* window;
	InputManager* inputManager;

	sf::Sprite playerSprite;
	sf::Texture playerTexture;

	float rotation;
	sf::Vector2f velocity;
	sf::FloatRect collisionBox;
};

