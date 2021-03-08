#include "Character.h"
#include <iostream>


Character::Character(sf::RenderWindow* hwnd, InputManager* input)
{
	window = hwnd;
	inputManager = input;

	playerTexture.loadFromFile("Resources/Spritesheet/spritesheet_characters.png", sf::IntRect(0, 0, 58, 44));
	setTexture(playerTexture);

	setOrigin(getTextureRect().width / 2.0f, getTextureRect().height / 2.0f);

	float collisionBoxSize = 32 * (window->getSize().y / 720.0f);
	collisionBox = sf::FloatRect(-collisionBoxSize / 2.0f, -collisionBoxSize / 2.0f, collisionBoxSize, collisionBoxSize);//sf::FloatRect(-16, -16, 32, 32);

	// Set the player to the same size as a tile in the tilemap then reduce it by 20 percent.
	setScale(sf::Vector2f((((40.0f / 58.0f) * 80.0f) / 100.0f), ((40.0f / 44.0f) * 80.0f) / 100.0f) * (float(window->getSize().x) / 1280.0f));
}


Character::~Character()
{
}

void Character::Update(float deltaTime, std::vector<int> tileMap)
{
	UpdateInput();

	setPosition(getPosition().x + (deltaTime * velocity.x), getPosition().y + (deltaTime * velocity.y));

	tileMapCollision(tileMap);

	Render();
}

void Character::UpdateInput()
{
	velocity = sf::Vector2f(0.f, 0.f);
	if (inputManager->getKey(sf::Keyboard::W))
		velocity.y = -150.f;
	if (inputManager->getKey(sf::Keyboard::A))
		velocity.x = -150.f;
	if (inputManager->getKey(sf::Keyboard::S))
		velocity.y = 150.f;
	if (inputManager->getKey(sf::Keyboard::D))
		velocity.x = 150.f;

	rotation = atan((getPosition().y - inputManager->getMouseY()) / (getPosition().x - inputManager->getMouseX())) * 180.08 / 3.14159f;

	if (getPosition().x < inputManager->getMouseX())
	{
		setRotation(rotation);
	}
	else
	{
		setRotation(rotation + 180);
	}
}

void Character::Render()
{
	window->draw(*this);
}

void Character::tileMapCollision(std::vector<int> tileMap)
{
	// Check collision with borders
	if (getCollisionBox().left < 0)
	{
		setPosition((getCollisionBox().width / 2.0f), getPosition().y);
		return;
	}
	if (getCollisionBox().left + getCollisionBox().width > window->getSize().x)
	{
		setPosition(window->getSize().x - (getCollisionBox().width / 2.0f), getPosition().y);
		return;
	}
	if (getCollisionBox().top < 0)
	{
		setPosition(getPosition().x, (getCollisionBox().height / 2.0f));
		return;
	}
	if (getCollisionBox().top + getCollisionBox().height > window->getSize().y)
	{
		setPosition(getPosition().x, window->getSize().y - (getCollisionBox().height / 2.0f));
		return;
	}

	float tileLeft;
	float tileTop;
	float tileWidthHeight = 40 *(window->getSize().y / 720.0f);
	float x1 = getCollisionBox().left + (getCollisionBox().width / 2.0f);
	float y1 = getCollisionBox().top + (getCollisionBox().height / 2.0f);
	float x2 = 0.f;
	float y2 = 0.f;
	float gradient = (y2 - y1) / (x2 - x1);

	// Check collision between the player and collidable tiles.
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (tileMap[i * 32 + j] != 0 && tileMap[i * 32 + j] != 95)
			{
				tileLeft = 40 * j;
				tileTop = 40 * i;

				if (getCollisionBox().left + getCollisionBox().width < tileLeft)
				{
					continue;
				}
				if (getCollisionBox().left > tileLeft + tileWidthHeight)
				{
					continue;
				}
				if (getCollisionBox().top + getCollisionBox().height < tileTop)
				{
					continue;
				}
				if (getCollisionBox().top > tileTop + tileWidthHeight)
				{
					continue;
				}

				x2 = tileLeft + (tileWidthHeight / 2.f);
				y2 = tileTop + (tileWidthHeight / 2.f);
				gradient = (y2 - y1) / (x2 - x1);

				if (gradient >= 1 || gradient <= -1)
				{
					if(y1 < y2)
						setPosition(getPosition().x, (y2 -/* 20.0f*/(tileWidthHeight / 2.0f)) - (getCollisionBox().height / 2.0f));
					else
						setPosition(getPosition().x, (y2 + /*20.0f*/(tileWidthHeight / 2.0f)) + (getCollisionBox().height / 2.0f));
				}
				else
				{
					if (x1 < x2)
						setPosition((x2 - (tileWidthHeight / 2.0f)/* 20.0f*/) - (getCollisionBox().width / 2.0f), getPosition().y);
					else
						setPosition((x2 + /*20.0f*/(tileWidthHeight / 2.0f)) + (getCollisionBox().width / 2.0f), getPosition().y);
				}
			}
		}
	}

}